from .gameteam import BasePlayer, BaseTeam, _ForceBeat, _ForceThink

class ClientPlayer(BasePlayer):
    PrivilegeForce = _ForceBeat

    def __init__(self, p):
        BasePlayer.__init__(self, p["id"], p["team"])
        self.x = p["x"]
        self.y = p["y"]
        self.score = p["score"]
        self.visible = True
        self.lastAction = None
        self.nextPos = (self.x, self.y)
        self.posStack = []  # 保留最近 n 次的位置
        self.visualArea = [[0, 0], [0, 0]]

    def distance(self, x, y):
        return abs(self.x - x) + abs(self.y - y)

    def isInvision(self, x, y):
        if abs(self.x - x) <= ClientTeam.Vision and abs(self.y - y) <= ClientTeam.Vision:
            return True
        return False

    def update(self, p):
        self.lastX = self.x
        self.lastY = self.y
        BasePlayer.__init__(self, p["id"], p["team"])
        self.x = p["x"]
        self.y = p["y"]
        self.score = p["score"]

    def __repr__(self):
        return '<Player [%s:%d (%d, %d)]> ' % (self.team, self.id, self.x, self.y)

class ClientTeam(BaseTeam):

    Width = 20
    Height = 20
    Vision = 3

    def __init__(self, id = 0):
        BaseTeam.__init__(self)
        self.meteor = None
        self.tunnel = None
        self.wormhole = None
        self.id = id

        # key 是 player 的 id， value 是 ClientPlayer 对象
        self.players = {}
        self.powers = [] # {'point': 1, 'x': 12, 'y': 2}
        self.playerNum = 0
        self.privilege = False

    def addPlayer(self, playerInfo, replace=True):
        player = ClientPlayer(playerInfo)
        if not replace and player.id in self.players:
            self.players[player.id].update(playerInfo)
        else:
            self.players[player.id] = player
        self.playerNum = len(self.players)

    def nearmate(self, x, y, dis = 5):
        count = 0
        for p in self.players.values():
            if abs(p.x - x) <= dis and abs(p.y - y) <= dis:
                count += 1
        return count

    def isMeteor(self, x, y):
        """Return True if meteor,
        if x, y is out of boundary, return True
        """
        for m in self.meteor:
            if x == m["x"] and y == m["y"]:
                return True

        if x < 0 or x >= ClientTeam.Width or y < 0 or y >= ClientTeam.Height:
            return True
        return False

    def isTunnel(self, x, y):
        """Return Tunnel Dict if found, else None
        {"x": 3, "y": 1, "direction": "down"}
        """
        for t in self.tunnel:
            if t["x"] == x and t["y"] == y:
                return t
        return None

    def isWormhole(self, x, y):
        """Return wormhole if found, else None
        {"name": "a","x": 4, "y": 1}
        """
        for wh in self.wormhole:
            if wh["x"] == x and wh["y"] == y:
                return wh
        return None

    def peerWormhole(self, wh):
        """Return Peer wormhole
        """
        name = wh["name"]
        if name.isupper():
            exp = name.lower()
        else:
            exp = name.upper()

        for h in self.wormhole:
            if h["name"] == exp:
                return h

    def isPlayer(self, x, y, exceptPlayer=None):
        for p in self.players.values():
            if p == exceptPlayer:
                continue
            if p.x == x and p.y == y:
                return p
            if p.nextPos[0] == x and p.nextPos[1] == y:
                return p
        return None

    def isPower(self, x, y):
        """Return Power point if found
        """
        for p in self.powers:
            if p["x"] == x and p["y"] == y:
                return p["point"]
        return None

    def tunnelNext(self, tunnel):
        """direct 是 tunnel 的方向，
        x y 是 tunnel 的位置
        """
        direct = tunnel["direction"]
        x = tunnel["x"]
        y = tunnel["y"]
        if direct == "right":
            x += 1
        elif direct == "left":
            x -= 1
        elif direct == "down":
            y += 1
        elif direct == "up":
            y -= 1
        # cascade tunnel
        t = self.isTunnel(x, y)
        if t is not None:
            return self.tunnelNext(t)
        return x, y

class ClientMap():
    pass