import json
from .sockserver import add_str_len
from .server import SimServer
from .mapelement import Team, Player
# add_str_len(json.dumps(msg)

RoundModeMap = {}
RoundModeMap[Team.ForceBeat] = "light" ## 火
RoundModeMap[Team.ForceThink] = "dark" 

import os

WormholeNameMap = {
    "A": 65,
    "a": 65,
    "B": 66,
    "b": 66,
    "C": 67,
    "c": 67,
    "D": 68,
    "d": 68,
}

class TraceSaver():
    RoundModeThink = "light"  
    RoundModeBeat  = "dark"
    # TODO save trace necessary
    def __init__(self, simserver: SimServer):
        self.ss = simserver
        try:
            # os.remove("replay.txt")
            self._file = open("replay.txt", "w")
        except:
            raise ValueError("File Error")
                  
    def close(self):
        self._file.close()

    def leg_start(self):
        belts = []
        for t in self.ss.mapObj.tunnel:
            belts.append({"dir": t["direction"],"x": t["x"],"y": t["y"]})

        gates = []
        for wh in self.ss.mapObj.wormhole:
            # name to char 65 == a
            n = WormholeNameMap[wh["name"]]
            gates.append({"name": n,"x": wh["x"], "y": wh["y"]})

        teams = [
            {   
                "team_id": self.ss.teamA.id,
                "team_name": self.ss.teamA.name,
                "team_player_id_list": self.ss.teamA.playerId
            },
            {
                "team_id": self.ss.teamB.id,
                "team_name": self.ss.teamB.name,
                "team_player_id_list": self.ss.teamB.playerId
            }
        ]

        walls = self.ss.mapObj.meteor

        block = {
            "belts": belts,
            "block_name": "leg_start",
            "gates": gates,
            "map_size": {
                "height": self.ss.mapObj.height,
                "width":  self.ss.mapObj.width
            },
            "max_round": SimServer.LegMaxRound,
            "mode": RoundModeMap[Player.PrivilegeForce],
            "switch_round": SimServer.ModeExchangRound,
            "teams": teams,
            "walls": walls
        }
        self._write(block)

    def round(self):
        mines = self.ss.mapObj.mines
        players = []
        for p in self.ss.mapObj.playerList:
            players.append(p.save_pack_round())

        teams = [
            self.ss.teamA.save_pack_round(),
            self.ss.teamB.save_pack_round()
        ]
        effects = {
            "belts": [],
            "create_mines": [],
            "distroy_mines": [],
            "kills": []
        }
        toRemove = []
        for k, v in effects.items():
            if len(v) == 0:
                toRemove.append(k)

        for k in toRemove:
            effects.pop(k)

        block = {
            "block_name": "round",
            "mines": mines,
            "mode": RoundModeMap[Player.PrivilegeForce],
            "players": players,
            "round_id": self.ss.round,
            "teams": teams,
            # "effects": effects
        }
        if len(effects) > 0:
            block["effects"] = effects
        self._write(block)

    def leg_end(self):
        block = {
            "block_name": "leg_end",
            "teams": [
                { "id": self.ss.teamA.id, "point": self.ss.teamA.point },
                { "id": self.ss.teamB.id, "point": self.ss.teamB.point },
            ]
        }
        self._write(block)
        
    def _write(self, msg, offset = 2):
        # offset 为 2，匹配原 server
        self._file.write(add_str_len(json.dumps(msg, separators=(',', ':')), offset = offset)+"\n")
