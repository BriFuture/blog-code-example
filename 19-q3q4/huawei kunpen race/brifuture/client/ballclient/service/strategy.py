from ballclient.service.clientteam import ClientPlayer, ClientTeam
from ballclient.util import logger
import random

"""
TODO 追击敌人时，多只鲲一起协作
2. 鲲原地打转问题

3. 增加一个可视区域，Privilege 尽量保证可视区域最大
4. 己方不能重叠

- 计算权重时，不考虑方向了
- 权重的计算公式
weight funciton (x-1)^3-x+2 or y= 4.25-5.04 x + 1.75 x^2
"""
class Strategy():
    Direction = ['right', 'left', 'down', 'up']
    WeightMeteor = 3
    WeightTunnel = 0.5
    WeightWormhole = 0.9
    WeightTunnelPeer = 1.2
    WeightWormholePeer = 2.0

    WeightTunnelUnprivilege   = 15
    WeightWormholeUnprivilege = 18
    WeightEnemyUnprivilege = 200
    WeightPower = 160
    WeightEnemy = 120

    WeightEmpty = 0.5

    RightHint = 0x800
    LeftHint  = 0x400
    DownHint  = 0x200
    UpHint    = 0x100

    RightDownHint = 0x80
    RightUpHint   = 0x40
    LeftDownHint  = 0x20
    LeftUpHint    = 0x10

    DownRightHint = 0x8
    DownLeftHint  = 0x4
    UpRightHint   = 0x2
    UpLeftHint    = 0x1

    def __init__(self, myteam, enemy):
        self._direction = None
        self.weight = 0
        self.myteam = myteam
        self.enemy = enemy
        self._weightDir = [0] * 4
        self.pos2 = [
            [1, 1], [2, 1], [1, 2], [2, 2]
        ]
        self.pos3 = [
            # fisrt vx, second vy
            [1, 1], [2, 1], [1, 2], [2, 2], 
            [3, 1], [1, 3], [3, 2], [2, 3], [3, 3]
        ]

    @property
    def direction(self):
        if self._direction is None:
            return []
        return [self._direction]

    def ableToGo(self, x, y):
        if self.myteam.isMeteor(x, y):
            # logger.debug(f"[R{self.round} - abletogo ({x}, {y})] Meteor False")
            return False
        if self.enemy.isPlayer(x, y) is not None:
            return self.myteam.privilege
        return True

    def _calcTunnelWeight(self, player, tunnel, direct, **kwargs):
        # TODO judge direction and next

        nx, ny = self.myteam.tunnelNext(tunnel)
        if nx == player.x and ny == player.y:
            # logger.debug(f"tunnel next is current loc")
            return - 100000
        tdis = abs(nx - player.x - 1) + abs(ny - player.y - 1) 
        if tdis > ClientTeam.Vision:
            tdis = abs(tdis - ClientTeam.Vision) + 1
        elif tdis == 0:
            tdis += 1
        # tmp = self._calcWeight(player, nx, ny, dis=tdis, trans=False, direct=direct)
        tmpList = [0, 0, 0, 0]
        # if (direct & Strategy.RightHint) == Strategy.RightHint:  # 右
        #     direct = 0x8c0
        # if (direct & Strategy.LeftHint) == Strategy.LeftHint:  # 左
        #     direct = 0x430
        # if (direct & Strategy.DownHint) == Strategy.DownHint:  # 下
        #     direct = 0x20c
        # if (direct & Strategy.UpHint) == Strategy.UpHint:  # 上
        #     direct = 0x103
        self.weightDirection(player, {"x": nx, "y": ny, "vx": 1, "vy": 1}, weightDir=tmpList, direct=direct, trans = False)
        # logger.debug(tmpList)
        tmp = sum(tmpList)
        if self.myteam.privilege == False:
            res = (tmp * Strategy.WeightTunnelPeer + random.randint(6, 8) * Strategy.WeightTunnelUnprivilege)
        else:
            res = (tmp * Strategy.WeightTunnelPeer + random.randint(2, 3) * Strategy.WeightTunnel)
        return res

    def _calcWormholeWeight(self, player, wormhole, direct):
        # TODO weight change
        pwh = self.myteam.peerWormhole(wormhole)
        nx = pwh["x"]
        ny = pwh["y"]

        tmpList = [0, 0, 0, 0]
        self.weightDirection(player, {"x": nx, "y": ny, "vx": 1, "vy": 1}, weightDir=tmpList, direct=0xfff)
        tmp = sum(tmpList)
        if self.myteam.privilege == False:
            return random.randint(10, 15) * Strategy.WeightWormholeUnprivilege + tmp * Strategy.WeightWormholePeer
        return random.randint(1, 2) * Strategy.WeightWormhole + tmp * Strategy.WeightWormholePeer

    def _calcWeightIgnoreDis(self, player, x, y, trans = True, direct = 0xfff):
        """x, y tried pos(next)
        @return weight
        不需要计算距离
        """
        tunnel = self.myteam.isTunnel(x, y)
        if tunnel is not None:
            if not trans:
                return Strategy.WeightEmpty
            # logger.debug(f"[R{self.round}] Tunnel ({x}, {y})")
            return self._calcTunnelWeight(player, tunnel, direct)

        wormhole = self.myteam.isWormhole(x, y)
        if wormhole is not None:
            # logger.debug(f"[R{self.round}] Wormhole ({x}, {y})")
            return self._calcWormholeWeight(player, wormhole, direct)

        # dis = pow(dis, 2)
        if self.myteam.isMeteor(x, y):
            # logger.debug(f"[R{self.round}] Meteor ({x}, {y})")
            return -0.5 * Strategy.WeightMeteor

        power = self.myteam.isPower(x, y)
        if power is not None:
            # logger.debug(f"[R{self.round}] Pos({x}, {y}) Power {power}")
            return power * Strategy.WeightPower

        ene = self.enemy.isPlayer(x, y)
        if ene is not None:
            if self.myteam.privilege == False:
                score =  (- ene.score - player.score) * Strategy.WeightEnemyUnprivilege - 80 * self.enemy.nearmate(x, y, dis=3)
            else:
                nearMate = self.myteam.nearmate(x, y, dis=ClientTeam.Vision) * self.myteam.remain_life
                score = (ene.score + 10) * Strategy.WeightEnemy * (0.5 + nearMate)
                # logger.debug(f"Tri to catch enemy({x}, {y}), near: {nearMate}")
            
            # logger.debug(f"[R{self.round}] Enemy ({x}, {y}) score: {score}")
            return score

        teammate = self.myteam.isPlayer(x, y, player)
        if teammate is not None:
            # TODO A -> X <- B
            return - 100

        return Strategy.WeightEmpty * random.randint(0, 3)  # empty

    def _calcWeight(self, player, x, y, dis = 1, trans = True, direct = 0xfff):
        return self._calcWeightIgnoreDis(player, x, y, trans, direct=direct) / dis

    def weightDirection(self, player, coord, weightDir, specdis = None, direct = 0xfff, **kwargs):
        """vx vy 都是距离，正值
        从目标点 (x, y) 开始，计算附近的位置的权值
        coord {"x": x, "y": y, "vx": vx, "vy": vy}
        weightDir array of 4 elements： weightDir 中的值会被累加
        """
        x  = coord["x"]
        y  = coord["y"]
        vx = coord["vx"]
        vy = coord["vy"]

        txr = x + vx   # 往右
        txl = x - vx   # 往左
        tyd = y + vy   # 往下
        tyu = y - vy   # 往上
        # 直线距离 4.25-5.04 x + 1.75 x^2
        otherDis = 1.75 * vx * vx - 5.04 * vx + 4.25
        if (direct & Strategy.RightHint) == Strategy.RightHint:  # 右
            weightDir[0] += self._calcWeight(player, txr, y, dis=otherDis, direct=Strategy.RightHint, **kwargs)           
        if (direct & Strategy.LeftHint) == Strategy.LeftHint:    # 左
            weightDir[1] += self._calcWeight(player, txl, y, dis=otherDis, direct=Strategy.LeftHint, **kwargs)           

        otherDis = 1.75 * vy * vy - 5.04 * vy + 4.25
        if (direct & Strategy.DownHint) == Strategy.DownHint:  # 下
            weightDir[2] += self._calcWeight(player, x, tyd, dis=otherDis, direct=Strategy.DownHint, **kwargs)           
        if (direct & Strategy.UpHint) == Strategy.UpHint:      # 上
            weightDir[3] += self._calcWeight(player, x, tyu, dis=otherDis, direct=Strategy.UpHint, **kwargs)  
        # otherDis = ((vx + vy - 0.5) * (vx + vy - 0.5) + abs(vx - vy) + 1)
        otherDis = 1.75 * (vx+vy) * (vx+vy) - 5.04 * (vx+vy) + 4.25
        if vy < vx or (vx < 2 and vy < 2):
            # 以横向为主
            if (direct & Strategy.RightDownHint) == Strategy.RightDownHint:
                weightDir[0] += self._calcWeight(player, txr, tyd, otherDis, **kwargs)  # 右下
            if (direct & Strategy.RightUpHint) == Strategy.RightUpHint:
                weightDir[0] += self._calcWeight(player, txr, tyu, otherDis, **kwargs)  # 右上

            if (direct & Strategy.LeftDownHint) == Strategy.LeftDownHint:
                weightDir[1] += self._calcWeight(player, txl, tyd, otherDis, **kwargs)  # 左下
            if (direct & Strategy.LeftUpHint) == Strategy.LeftUpHint:
                weightDir[1] += self._calcWeight(player, txl, tyu, otherDis, **kwargs)  # 左上
        elif vy > vx or (vx < 2 and vy < 2):
            # 以纵向为主
            if (direct & Strategy.DownRightHint) == Strategy.DownRightHint:
                weightDir[2] += self._calcWeight(player, txr, tyd, otherDis, **kwargs)  # 下右
            if (direct & Strategy.DownLeftHint) == Strategy.DownLeftHint:
                weightDir[2] += self._calcWeight(player, txl, tyd, otherDis, **kwargs)  # 下左

            if (direct & Strategy.UpRightHint) == Strategy.UpRightHint:
                weightDir[3] += self._calcWeight(player, txr, tyu, otherDis, **kwargs)  # 上右
            if (direct & Strategy.UpLeftHint) == Strategy.UpLeftHint:
                weightDir[3] += self._calcWeight(player, txl, tyu, otherDis, **kwargs)  # 上左
        else:
            pass           


    def compare(self, player):
        """
        """
        mweight = -1000000
        d = -1

        if player.lastAction is not None and self._weightDir[0] == self._weightDir[1] and self._weightDir[1] == self._weightDir[2] \
            and self._weightDir[2] == self._weightDir[3]:
            self._weightDir[Strategy.Direction.index(player.lastAction)] += 30

        for i in range(4):
            if mweight < self._weightDir[i]:
                mweight = self._weightDir[i]
                d = i

        logger.debug("[R%d  %s - %s] <===> %s"
            % (self.round, player, Strategy.Direction[d], self._weightDir))
        return Strategy.Direction[d]

    def makeAction(self, action, round):
        self.round = round

        for player in self.myteam.players.values():
            # logger.debug(f"[R{self.round}] {player}")
            x = player.x
            y = player.y
            # iterate
            # 四种组合，先右后左，先下后上
            for i in range(4):
                self._weightDir[i] = 0

            if not self.ableToGo(x+1, y):
                self._weightDir[0] = -100000
            if not self.ableToGo(x-1, y):
                self._weightDir[1] = -100000
            
            if not self.ableToGo(x, y+1):
                self._weightDir[2] = -100000
            if not self.ableToGo(x, y-1):
                self._weightDir[3] = -100000
            
            for p in self.pos3:
                self.weightDirection(player, 
                    coord = {"x": player.x, "y":player.y, "vx": p[0], "vy": p[1]}, 
                    weightDir=self._weightDir,
                    direct=0xfff)
            player.lastAction = self.compare(player)
            player.nextPos = self.playerNextPos(player)
            # if len(player.posStack) < 5:
        
        for player in self.myteam.players.values():
            # logger.debug(f"[R{self.round} {self.myteam.privilege}]{player}: {res}")
            action.append({"team": player.team, "player_id": player.id,
                "move": [player.lastAction]})

    def playerNextPos(self, player):
        if player.lastAction == 'right':
            return (player.x + 1, player.y)
        elif player.lastAction == 'left':
            return (player.x - 1, player.y)
        elif player.lastAction == 'up':
            return (player.x, player.y - 1)
        elif player.lastAction == 'down':
            return (player.x, player.y + 1)


class Strategy2(Strategy):

    def __init__(self, myteam, enemy):
        Strategy.__init__(self, myteam, enemy)
        self.pos3 = [
            # fisrt vx, second vy
            [1, 0], [0, 1], [1, 1], 
            [2, 0], [0, 2], [2, 1], [1, 2], [2, 2], 
            [3, 0], [0, 3], [3, 1], [1, 3], [3, 2], [2, 3], [3, 3]
        ]

    def makeAction(self, action, round):
        self.round = round

        for player in self.myteam.players.values():
            # logger.debug(f"[R{self.round}] {player}")
            x = player.x
            y = player.y
            # iterate
            # 四种组合，先右后左，先下后上
            for i in range(4):
                self._weightDir[i] = 0

            if not self.ableToGo(x+1, y):
                self._weightDir[0] = -100000
            if not self.ableToGo(x-1, y):
                self._weightDir[1] = -100000
            
            if not self.ableToGo(x, y+1):
                self._weightDir[2] = -100000
            if not self.ableToGo(x, y-1):
                self._weightDir[3] = -100000
            
            for p in self.pos3:
                coord = {"x": player.x, "y":player.y, "vx": p[0], "vy": p[1]}
                coord["tx"] = player.x + p[0] 
                coord["ty"] = player.y + p[1]
                self.weightPos(player, coord = coord, weightDir=self._weightDir, direct=0xfff)
                coord["tx"] = player.x + p[0] 
                coord["ty"] = player.y - p[1]
                self.weightPos(player, coord = coord, weightDir=self._weightDir, direct=0xfff)
                coord["tx"] = player.x - p[0] 
                coord["ty"] = player.y + p[1]
                self.weightPos(player, coord = coord, weightDir=self._weightDir, direct=0xfff)
                coord["tx"] = player.x - p[0] 
                coord["ty"] = player.y - p[1]
                self.weightPos(player, coord = coord, weightDir=self._weightDir, direct=0xfff)

            player.lastAction = self.compare(player)
            player.nextPos = self.playerNextPos(player)
            # if len(player.posStack) < 5:
        # TODO make a controller to control all actions
        
        for player in self.myteam.players.values():
            # logger.debug(f"[R{self.round} {self.myteam.privilege}]{player}: {res}")
            action.append({"team": player.team, "player_id": player.id,
                "move": [player.lastAction]})
    
    def _calcWeightPos(self, player, x, y, dis=1, dir=-1, trans=True):
        tunnel = self.myteam.isTunnel(x, y)
        if tunnel is not None:
            if not trans:
                return Strategy.WeightEmpty
            # logger.debug(f"[R{self.round}] Tunnel ({x}, {y})")
            return self._calcTunnelWeight(player, tunnel, 0xfff)

        wormhole = self.myteam.isWormhole(x, y)
        if wormhole is not None:
            # logger.debug(f"[R{self.round}] Wormhole ({x}, {y})")
            return self._calcWormholeWeight(player, wormhole, 0xfff)

        # dis = pow(dis, 2)
        if self.myteam.isMeteor(x, y):
            # logger.debug(f"[R{self.round}] Meteor ({x}, {y})")
            return -0.5 * Strategy.WeightMeteor

        power = self.myteam.isPower(x, y)
        if power is not None:
            # logger.debug(f"[R{self.round}] Pos({x}, {y}) Power {power}")
            return power * Strategy.WeightPower

        ene = self.enemy.isPlayer(x, y)
        if ene is not None:
            if self.myteam.privilege == False:
                score =  (- ene.score - player.score) * Strategy.WeightEnemyUnprivilege - 80 * self.enemy.nearmate(x, y, dis=3)
            else:
                nearMate = self.myteam.nearmate(x, y, dis=ClientTeam.Vision) * self.myteam.remain_life
                score = (ene.score + 10) * Strategy.WeightEnemy * (0.5 + nearMate)
                # logger.debug(f"Tri to catch enemy({x}, {y}), near: {nearMate}")
            
            # logger.debug(f"[R{self.round}] Enemy ({x}, {y}) score: {score}")
            return score

        return Strategy.WeightEmpty * random.randint(0, 3)  # empty
        
    def weightPos(self, player, coord, weightDir, **kwargs):
        """
        coord x, y 起始点坐标， vx vy 距离，可以为 0，tx ty 目标点坐标
        """
        x  = coord["x"]
        y  = coord["y"]
        vx = coord["vx"]
        vy = coord["vy"]
        tx = coord["tx"]
        ty = coord["ty"]
        distance = 1.75 * (vx+vy) * (vx+vy) - 5.04 * (vx+vy) + 4.25
        dir = self.judegDir(x, y, vx, vy, tx, ty)
        if dir[0] > -1:
            weightDir[dir[0]] += self._calcWeightIgnoreDis(player, tx, ty) / distance
        if dir[1] > -1:
            weightDir[dir[1]] += self._calcWeightIgnoreDis(player, tx, ty) / distance

    def judegDir(self, x, y, vx, vy, tx, ty):
        dirX = -1
        dirY = -1
        # warning: vy == vx 出现了两次
        if vy <= vx:
            if tx > x:
            # right
                dirX = 0
            elif tx < x:
            # left
                dirX = 1
            # return dirX
        if vx <= vy:
            if ty > y:
            # down
                dirY = 2
            elif ty < y:
            # up
                dirY = 3
            # return dirY
        return dirX, dirY