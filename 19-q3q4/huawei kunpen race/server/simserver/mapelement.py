from .gameteam import BasePlayer, BaseTeam, _ForceBeat, _ForceThink, BasePower
from .util import logger

class Player(BasePlayer):
    PrivilegeForce = _ForceBeat

    """包含 id， teamid，位置（x y）
    """
    def __init__(self, id: int, teamid: int):
        BasePlayer.__init__(self, id, teamid)


    def setAction(self, action):
        if action in Player.ActionNormal:
            self.action = action
        else:
            self.action = Player.ActionNone

    def triMove(self):
        """According to action, change x, y
        """
        return Player.testMove(self.x, self.y, self.action)

    @staticmethod
    def playerKillAnother(killer: BasePlayer, killed: BasePlayer):
        """Player killer killed Player killed
        """
        killer.score += killed.score
        killed.score = 0
        killed._sleep = 1
        killed.group.remain_life -= 1

    

    def eatPower(self, power: BasePower):
        self.score += power.value
        logger.debug(f"===> {self} eat {power}")
        power.exist = False

    def meetPlayer(self, player: BasePlayer):
        if player.team == self.team:
            return
        
        # TODO
        if player.force == Player.PrivilegeForce:
            Player.playerKillAnother(player, self)
        else:
            Player.playerKillAnother(self, player)

    def pack(self):
        return { 
            "id": self.id, 
            "score": self.score, 
            "sleep": self._sleep,
            "team": self.team, 
            "x": self.x, 
            "y": self.y,
            # "status": self.status
        }

    def save_pack_round(self):
        return {
            # "moveOn": self.action, 
            "moveOn": "blank", 
            "player_id": self.id,
            "team_id": self.team, 
            "point": self.score,
            "sleep": self._sleep,
            "status": self.status,
            "x": self.x,
            "y": self.y
        }
    
    def __repr__(self):
        return f'<Player [{self.team}:{self.id} ({self.x}, {self.y})] action: {self.action}> '

class Team(BaseTeam):
    """Team
    """
    # 视野
    Vision = 3
    def __init__(self):
        BaseTeam.__init__(self)

    def getPlayer(self, pid):
        """Return player if found
        """
        for p in self.players:
            if p.id == pid:
                return p
        return None

    def setForce(self, force):
        self.force = force
        for p in self.players:
            p.force = force

    def addPlayer(self, pid: int, x: int, y: int):
        p = Player(pid, self.id)
        p.x = x
        p.y = y
        p.force = self.force
        p.group = self
        self.players.append(p)
        self.playerId.append(pid)
        return p

    def round_player_pack(self, another: BaseTeam):
        """返回当前队伍的 Player 及能够看到的对方队伍的 Player
        """
        pack = []
        for p in self.players:
            pack.append(p.pack())
        for ap in another.players:
            for p in self.players:
                if abs(p.x - ap.x) <= Team.Vision and abs(p.y - ap.y) <= Team.Vision:
                    pack.append(ap.pack())
                    break
        return pack

    def __repr__(self):
        return f'<Team {self.id}, point: {self.point}>'

class Power(BasePower):
    def __init__(self, id, x, y):
        BasePower.__init__(self, id, x, y)

    def isVisibleToPlayer(self, player: Player) -> bool:
        if abs(player.x - self.x) <= Team.Vision and abs(player.y - self.y) <= Team.Vision:
            return True
        return False

    def pack(self):
        return {
            "point": self.value,
            "x": self.x,
            "y": self.y
        }

    def save_pack(self):
        m = {
            "id": self.id,
            "value": self.value,
            "x": self.x,
            "y": self.y
        }
        return m

    def __repr__(self):
        return f'<Power ({self.x}, {self.y}) [{self.value}]>'
