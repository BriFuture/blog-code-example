from .gameteam import BasePlayer, BaseTeam, _ForceBeat, _ForceThink, BasePower
from ballclient.util import logger

class Player(BasePlayer):
    PrivilegeForce = _ForceBeat

    """包含 id， teamid，位置（x y）
    """
    def __init__(self, id, teamid):
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
    def playerKillAnother(killer, killed):
        """Player killer killed Player killed
        """
        killer.score += killed.score
        killed.score = 0
        killed._sleep = 1
        killed.group.remain_life -= 1


    def meetPlayer(self, player):
        if player.team == self.team:
            return
        
        # TODO
        if player.force == Player.PrivilegeForce:
            Player.playerKillAnother(player, self)
        else:
            Player.playerKillAnother(self, player)

    
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

    def addPlayer(self, pid, x, y):
        p = Player(pid, self.id)
        p.x = x
        p.y = y
        p.force = self.force
        p.group = self
        self.players.append(p)
        self.playerId.append(pid)
        return p

    def round_player_pack(self, another):
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

    def isVisibleToPlayer(self, player):
        if abs(player.x - self.x) <= Team.Vision and abs(player.y - self.y) <= Team.Vision:
            return True
        return False

    def __repr__(self):
        return f'<Power ({self.x}, {self.y}) [{self.value}]>'
