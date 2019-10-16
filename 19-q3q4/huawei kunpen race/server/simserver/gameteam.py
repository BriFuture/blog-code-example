_ForceBeat = "beat"
_ForceThink = "think"

class BasePlayer():
    StatusAlive = "alive"
    StatusDead  = "Dead"

    ActionNone = "blank"
    ActionNormal = ["up", "down", "left", "right"]

    def __init__(self, id: int, teamid: int):
        self.score = 0
        self._sleep = 0
        self.x = 0
        self.y = 0
        self.action = BasePlayer.ActionNone
        self.force = None
        self.status = BasePlayer.StatusAlive
        self.id = id
        self.team = teamid
        self.group = None

    @property
    def sleep(self):
        return 1 if self._sleep else 0
        
    @staticmethod
    def testMove(x, y, action):
        if action == "up":
            return x, y - 1
        elif action == "down":
            return x, y + 1
        elif action == "left":
            return x - 1, y
        elif action == "right":
            return x + 1, y
        else:
            return x, y

    def _beforeMove(self):
        self.lastX = self.x
        self.lastY = self.y

    def undoMove(self):
        self.x = self.lastX
        self.y = self.lastY

    def moveUp(self):
        self._beforeMove()
        self.y -= 1

    def moveDown(self):
        self._beforeMove()
        self.y += 1

    def moveLeft(self):
        self._beforeMove()
        self.x -= 1

    def moveRight(self):
        self._beforeMove()
        self.x += 1

class BaseTeam():
    ForceBeat = _ForceBeat
    ForceThink = _ForceThink

    def __init__(self):
        self.id = 0
        self.conn = None
        self.name = None
        self.remain_life = 0
        self.registered = False
        self.players = []
        self.playerId = []
        self.force = BaseTeam.ForceBeat
    
    @property
    def point(self):
        po = 0
        for p in self.players:
            po += p.score
        return po

    
    def save_pack_round(self):
        return {
            "point": self.point,
            "remain_life": self.remain_life,
            "team_id": self.id
        }

    def leg_start_pack(self, force):
        self.setForce(force)
        p = {
            "id": self.id,
            "players": self.playerId,
            "force": self.force
        }
        return p

    def setForce(self, force):
        self.force = force

import random

class BasePower():
    def __init__(self, id, x, y):
        self.id = id
        self.x  = x
        self.y  = y
        self.value = random.randint(1, 5)
        self.exist = True