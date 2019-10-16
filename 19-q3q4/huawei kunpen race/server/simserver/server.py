"""
"""

import json
from .util import logger
import traceback
from .gamemap import GameMap, Player, Team
from .sockserver import SockServer, sendMsg
import time

class BaseSimServer():
    ModeExchangRound = 10
    LegMaxRound = 20
    # ModeExchangRound = 150
    # LegMaxRound = 300
    TotalLife = 4

    DebugMode = True

    PlayerAIdStart = 0
    PlayerBIdStart = 4

    def __init__(self, port: int, mapFile: str):
        self._ss = SockServer(port)
        self.teamA = Team()
        self.teamB = Team()
        self.mapFile = mapFile
        self.mapObj = GameMap(mapFile)
        BaseSimServer.TotalLife = self.mapObj.map["game"]["revive_times"]
        self.round = 0
        self.teamCount = 0

        from .tracesaver import TraceSaver
        self.traceSaver = TraceSaver(self)

    def save_gamecsvFile(self):
        content = [self.mapFile, self.teamA.name, self.teamA.id, self.teamA.point,
            self.teamB.name, self.teamB.id, self.teamB.point]
        with open("game.csv", "w") as f:
            logger.debug(f"content  write: {content}")
            f.write(",".join(map(str, content)))

    def start(self):
        data = None
        try:
            # start of registration
            # wait Team A
            while self.teamCount < 2:
                if not self.teamA.registered:
                    self._register(self.teamA, self.mapObj.teamA, BaseSimServer.PlayerAIdStart)

                # wait Team B
                if not self.teamB.registered:
                    self._register(self.teamB, self.mapObj.teamB, BaseSimServer.PlayerBIdStart)

            logger.info(f"===========Registration finished=========")

            self.save_gamecsvFile()

            # end of registration

            self._loop_leg()

            # send game over
            self._broadcast({"msg_name": "game_over"})
            logger.info(f"Game Over, A: {self.teamA}, B: {self.teamB}")
        except Exception as e:
            logger.warning(f"[Start] Error: {e}, data: {data}")
            traceback.print_exc()
        finally:
            self.traceSaver.close()
            self._ss.sock.close()

        # if SimServer.DebugMode:
        #     self.start()

    def _register(self, team: Team, initPos: dict, offset: int):
        conn, _ = self._ss.accept()
        team.conn = conn
        data = self._ss.recv(conn)
        # logger.debug(data)
        if data["msg_name"] != "registration":
            logger.info(f"Expect Registration, not recved: {data}")
            return
        team.id = data["msg_data"]["team_id"]
        team.name = data["msg_data"]["team_name"]
        team.remain_life = SimServer.TotalLife
        team.registered = True

        for i, xy in enumerate(initPos):
            p = team.addPlayer(i + offset, xy["x"], xy["y"])
            self.mapObj.addPlayer(i + offset, p)
            logger.info(f"Player created: {p}")

        logger.info(f"Registered: {team.id}, pos inited")
        self.teamCount += 1
            
    def _leg_start(self, aForce, bForce):
        teams = [
            self.teamA.leg_start_pack(aForce),
            self.teamB.leg_start_pack(bForce)
        ]
        logger.debug(f"leg_start: {teams}")
        map = {
            "width":  self.mapObj.width,
            "height": self.mapObj.height,
            "vision": Team.Vision,
            "meteor": self.mapObj.meteor,
            "tunnel": self.mapObj.tunnel,
            "wormhole": self.mapObj.wormhole
        }
        msg = {
            "msg_name": "leg_start",
            "msg_data": {
                "map": map, 
                "teams": teams
            }
        }
        self._sendA(msg)
        self._sendB(msg)
        self.traceSaver.leg_start()

    def _loop_leg(self):
        pass
      
 
    def _leg_end(self):
        msg = {
            "msg_name": "leg_end",
            "msg_data": {
                "teams": [
                    { "id": self.teamA.id, "point": self.teamA.point },
                    { "id": self.teamB.id, "point": self.teamB.point },
                ]
            }
        }
        self._broadcast(msg)
        self.traceSaver.leg_end()

    def _sendA(self, msg):
        return sendMsg(self.teamA.conn, msg)
    
    def _sendB(self, msg):
        return sendMsg(self.teamB.conn, msg)

    def _broadcast(self, msg):
        self._sendA(msg)
        self._sendB(msg)



class SimServer(BaseSimServer):

    def __init__(self, port : int, mapFile: str):
        BaseSimServer.__init__(self, port, mapFile)
        self.running = False
        self.leg = 0
        Player.PrivilegeForce = Team.ForceBeat
        # self.mapObj.addPowers(20)

    def _loop_leg(self, whole = False):
        # start of leg start
        self._leg_start(Team.ForceBeat, Team.ForceThink)  
        # 上半场， 先火后水
        self._half_leg_mode(Team.ForceBeat)            # mode A
        self._half_leg_mode(Team.ForceThink, SimServer.ModeExchangRound) # mode B
        for p in self.mapObj.playerList:
            p.setAction("")
        self.traceSaver.round()
        self._leg_end() #  send leg end
        logger.info(f"End Start Leg 1")

        if not whole:
            return
        # start of leg start
        self._leg_start(Team.ForceThink, Team.ForceBeat)
        # 下半场 先水后火
        self._half_leg_mode(Team.ForceThink)             # mode A
        self._half_leg_mode(Team.ForceBeat, SimServer.ModeExchangRound) # mode B
        self._leg_end() #  send leg end
        logger.info(f"End Start Leg 2")

    def _half_leg_mode(self, privilegeForce, roundOff = 0):
        # loop round
        self.round = roundOff
        Player.PrivilegeForce = privilegeForce
        logger.info(f"PrivilegeForce is {privilegeForce}")
        # privilegeForce == Team.ForceBeat and teamA.force is Team.ForceBeat, A 是优势
        firstTeam = self.teamA
        secondTeam = self.teamB
        if self.teamA.force == Player.PrivilegeForce:
            firstTeam = self.teamB
            secondTeam = self.teamA
        while self.round < SimServer.ModeExchangRound + roundOff:
            logger.debug(f"-----Round: {self.round}----")
            
            self.traceSaver.round()

            self._teamRound(firstTeam, secondTeam)
            aData = self._ss.recv(firstTeam.conn)
            aData["team"] = firstTeam  # for debug
            self._recv_actions(aData)
            self._move_team(firstTeam)

            self._teamRound(secondTeam, firstTeam)
            bData = self._ss.recv(secondTeam.conn)
            bData["team"] = secondTeam  # for debug
            self._recv_actions(bData)
            self._move_team(secondTeam)

            logger.debug(f"------actionA - {aData}------\n----actionB - {bData}-----")

            # TODO uncomment following
            if self.round % 8 == 0 and self.mapObj.powerLen < 15:
                self.mapObj.addPowers(self.mapObj.map["game"]["power_create_num"])

            # input("wait")
            # if aData["round_id"]: # check round id
            
            self.round += 1
            time.sleep(0.01)
        # end of half_leg, mode needs exchange
   
    def _teamRound(self, team: Team, another: Team):
        """Send msg to team, another for reference
        """
        players = team.round_player_pack(another)

        teams  = [
            { "id": team.id, "point": team.point, "remain_life": team.remain_life},
            { "id": another.id, "point": another.point, "remain_life": another.remain_life}
        ]
        msg = {
            "msg_name": "round",
            "msg_data": {
                "round_id": self.round ,
                "mode":  Player.PrivilegeForce,
                "power": self.mapObj.getPowers(team),
                "players": players,
                "teams": teams
            }
        }
        logger.debug(f"---Round---: {team} Recv [{msg}]")
        sendMsg(team.conn, msg)        

    def _recv_actions(self, action: dict):
        if action["msg_name"] != "action":
            logger.warning(f"Round: {self.round}, Recved msg not action, msg: {action}")
            return
        for act in action["msg_data"]["actions"]:
            if act["team"] != action["team"].id: continue

            pid = act["player_id"]
            player = action["team"].getPlayer(pid)
            player_actname = ""
            if len(act["move"]) == 1:
                player_actname = act["move"][0]

            player.setAction(player_actname)

    def _move_team(self, team: Team):
        for p in team.players:
            self._move_player(p)
            
    def _move_player(self, player: Player):
        if player.action == Player.ActionNone:  return

        # 获取下一个位置
        x, y = player.triMove()
        if x < 0 or x >= self.mapObj.height \
            or y < 0 or y >= self.mapObj.width \
            or self.mapObj.isMeteor(x, y):
            return
        player.x = x
        player.y = y
        if self.mapObj.isTunnel(x, y):
            t = self.mapObj.tunnelType(x, y)
            player.setAction(t)
            # tunnel
            self._move_player(player)
            return

        if self.mapObj.isWormhole(x, y) and player.action is not Player.ActionNone:
            whpeer = self.mapObj.wormholePeer(x, y)
            player.x = whpeer["x"]
            player.y = whpeer["y"]
            return

        power = self.mapObj.isPower(x, y)
        if power is not None:
            player.eatPower(power)
            return

        another = self.mapObj.isPlayer(x, y)
        if another is not None:
            player.meetPlayer(another)
            return
