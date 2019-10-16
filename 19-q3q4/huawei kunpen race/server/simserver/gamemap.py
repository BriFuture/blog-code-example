import json
import random

from .util import logger
from .mapelement import Power, Player, Team

class BaseGameMap(object):
    PosEmpty = "."
    PosMeteor = "#"

    PosTunnelUp = "^"
    PosTunnelDown = "v"
    PosTunnelLeft = "<"
    PosTunnelRight = ">"

    PosWormholeA = "A"
    PosWormholea = "a"
    PosWormholeB = "B"
    PosWormholeb = "b"
    PosWormholeC = "C"
    PosWormholec = "c"
    PosWormholeD = "D"
    PosWormholed = "d"

    PosTeamA = "O"
    PosTeamB = "X"

    PosTunnels = [PosTunnelUp, PosTunnelDown, PosTunnelLeft, PosTunnelRight]
    PosWormhole  = [PosWormholeA, PosWormholea, PosWormholeB, PosWormholeb,
                    PosWormholeC, PosWormholec, PosWormholeD, PosWormholed]

    PosTeam = [PosTeamA, PosTeamB]

    PowerCreateNum = 5
    def __init__(self):
        ## find meteor, tunnel, wormhole
        self.meteor = []
        self.tunnel = []
        self.wormhole = []
        self.teamA = []
        self.teamB = []
        self._map_str_list = None
        self._powers = []
        self.powerId = 30
    
    def _pushMeteor(self, i: int, j: int):
        m = {"x": i, "y": j}
        self.meteor.append(m)
    
    def _pushTunnel(self, i: int, j: int, s: str):
        d = GameMap.PosTunnels.index(s)
        m = {"x": i, "y": j, "direction": GameMap.PosTunnelDesc[d]}
        # print(m, s)
        self.tunnel.append(m)

    def _pushWormhole(self, i: int, j: int, s: str):
        m = {"name": s, "x": i, "y": j}
        self.wormhole.append(m)

    def _pushTeam(self, i: int, j: int, s: str):
        t = {"x": i, "y": j}
        if s == GameMap.PosTeamA :
            self.teamA.append(t)
        elif s == GameMap.PosTeamB:
            self.teamB.append(t)

    def getPosStr(self, x: int, y: int):
        return self._map_str_list[y][x]

    def isMeteor(self, x: int, y: int):
        """True or False
        """
        return self._map_str_list[y][x] == GameMap.PosMeteor

    def isTunnel(self, x: int, y: int):
        """True or False
        """
        return self._map_str_list[y][x] in GameMap.PosTunnels

    def isWormhole(self, x: int, y: int):
        """True or False
        """
        return self._map_str_list[y][x] in GameMap.PosWormhole

    def wormholePeer(self, x: int, y: int):
        """Return Wormhole Dict peers to (x, y)
        For example, if a is at (x, y), then return A
        """
        wh = self._map_str_list[y][x]
        if wh.isupper():
            exp = wh.lower()
        else:
            exp = wh.upper()

        for h in self.wormhole:
            if h["name"] == exp:
                return h
        return None        

    def addPowers(self, count = -1):
        if count == -1:
            count = self.PowerCreateNum
        pc = 0
        while pc < count:
            i = random.randint(0, self.height - 1)
            j = random.randint(0, self.height - 1)

            if self.isMeteor(i, j) or self.isTunnel(i, j) \
                or self.isWormhole(i, j) or self.isPlayer(i, j) \
                or self.isPower(i, j):
                continue

            if random.random() > 0.8:
                found = False
                for po in self._powers:
                    if po.x == i and po.y == j:
                        found = True
                if found: continue
                
                self.addPower(i, j)
                pc += 1
                logger.debug(f"generate Power at ({i}, {j})")

    def isPower(self, x: int, y: int) -> Power:
        """Return Power action if True or None will return
        """
        for p in self._powers:
            if x == p.x and y == p.y and p.exist:
                return p
        return None

    @property
    def powers(self):
        return [x.pack() for x in self._powers] 

    @property
    def powerLen(self):
        return len(self._powers)


    def tunnelType(self, x: int, y: int):
        t = self._map_str_list[y][x]
        if t == GameMap.PosTunnelUp:
            return "up"
        elif t == GameMap.PosTunnelDown:
            return "down"
        elif t == GameMap.PosTunnelLeft:
            return "left"
        elif t == GameMap.PosTunnelRight:
            return "right"

    def addPower(self, i, j, value = None):
        p = Power(self.powerId, i, j)
        self.powerId += 1
        self._powers.append(p)
        if value is not None:
            p.value = value

    def _parse(self, map_str_list: list, width: int):
        self.width = width
        self.height = width                    

        for j, map_str in enumerate(map_str_list):
            for i in range(width):
                if map_str[i] == GameMap.PosMeteor:
                    self._pushMeteor(i, j)
                elif map_str[i] in GameMap.PosTunnels:
                    self._pushTunnel(i, j, map_str[i])
                elif map_str[i] in GameMap.PosWormhole:
                    self._pushWormhole(i, j, map_str[i])
                elif map_str[i] in GameMap.PosTeam:
                    self._pushTeam(i, j, map_str[i])
                elif map_str[i] == BaseGameMap.PosEmpty:
                    continue
                else:
                    try:
                        value = int(map_str[i])
                        self.addPower(i, j, value)
                    except Exception as e:
                        logger.warning(e)


class GameMap(BaseGameMap):
    """
    Property: meteor, tunnel, wormhole, 
        teamInitPos(teamA, teamB), vision, height
    """
    
    PosTunnelDesc = ["up", "down", "left", "right"]

    def __init__(self, mapFile: str):
        BaseGameMap.__init__(self)
        self.map = {}
        self._players = {}
        with open(mapFile) as f:
            f.readline()
            content = ""
            while True:
                text = f.readline().strip()
                content += text
                # logger.debug(text)
                if text.find("},") > -1:
                    break
            # logger.debug(content[7:-1])
            self.map["game"] = json.loads(content[7:-1])
            Team.Vision = self.map["game"]["vision"]
            BaseGameMap.PowerCreateNum =self.map["game"]["power_create_num"]
            self.map["map"] = {}
            for text in f:
                text = text.strip()
                if text.find("height") > -1:
                    i = text.index("height")
                    self.map["map"]["height"] = int(text[i + 2 +len("length"):-1])
                if text.find("width") > -1:
                    i = text.index("width")
                    self.map["map"]["width"] = int(text[i + 1 +len("length"):-1])
                if text.find("map_str") > -1:
                    break
            # map_str_list 每一行存放的是 该行的 map 信息
            map_str_list = []
            
            for i, text in enumerate(f):
                text = text.strip()
                if text.find("\"") > -1: break
                map_str_list.append(text)
                i += 1
            # logger.debug(map_str_list)
            self._parse(map_str_list, self.map["map"]["width"])
            self._map_str_list = map_str_list
        
    def isPlayer(self, x: int, y: int):
        """Return Player Instance if there is, or None will be returned
        """
        for p in self._players.values():
            if x == p.x and y == p.y:
                return p
        return None


    def addPlayer(self, pid, player: Player):
        self._players[pid] = player


    def getPowers(self, team: Team):
        power = []
        for po in self._powers:
            see = False
            for pe in team.players:
                # pe = Player()
                if pe.status == Player.StatusDead:
                    continue
                if po.isVisibleToPlayer(pe):
                    see = True
                    break
            if see:
                power.append(po.pack())
        return power

    @property
    def mines(self):
        return [x.save_pack() for x in self._powers if x.exist]

    @property
    def playerList(self):
        return self._players.values()

    def eatPower(self, player: Player):
        """dep
        """
        dp = None
        for power in self._powers:
            if power["x"] == player.x and power["y"] == player.y:
                player.score += power.value
                dp = power      
                break
        if dp is not None:
            self._powers.remove(dp)
        