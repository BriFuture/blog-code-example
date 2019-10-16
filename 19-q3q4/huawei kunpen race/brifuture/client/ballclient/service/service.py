# encoding:utf8
'''
业务方法模块，需要选手实现

选手也可以另外创造模块，在本模块定义的方法中填入调用逻辑。这由选手决定

所有方法的参数均已经被解析成json，直接使用即可

所有方法的返回值为dict对象。客户端会在dict前面增加字符个数。
'''
import ballclient.service.constants as constants
from ballclient.util import logger
from ballclient.service.clientteam import ClientPlayer, ClientTeam
from ballclient.service.strategy import Strategy, Strategy2

myteam = ClientTeam(constants.team_id)
enemy = ClientTeam()
strategy = Strategy2(myteam, enemy)

def leg_start(msg):
    '''
    :param msg:
    :return: None
    '''
    msg_data = msg["msg_data"]
    msg_map = msg_data['map']
    ClientTeam.Vision = msg_map['vision']
    ClientTeam.Width  = msg_map['width']
    ClientTeam.Height = msg_map['height']

    for t in msg_data['teams']:
        if t["id"] == constants.team_id:
            for pid in t['players']:
                myteam.addPlayer({"score": 0, "x": 0, "y": 0, "id": pid, "team": myteam.id})
            ClientPlayer.PrivilegeForce = t["force"]
        else:
            enemy.id = t["id"]
    
    myteam.meteor = msg_map['meteor']
    myteam.tunnel = msg_map['tunnel']
    myteam.wormhole = msg_map['wormhole']

    # logger.debug(f"Leg start: {msg_data}")

def round(msg):
    '''
    :param msg: dict
    :return:
    return type: dict
    '''
    msg_data = msg['msg_data']
    round_id = msg_data['round_id']
    action = []
    
    if 'players' in msg_data:
        players = msg_data['players']
        # logger.debug("========Code Needs Update=======")
        enemy.players = {}
        if 'power' in msg_data:
            myteam.powers = msg_data['power']
        else:
            myteam.powers = []
        mode = msg_data["mode"]
        myteam.privilege = (mode == ClientPlayer.PrivilegeForce)
        for team in msg_data["teams"]:
            if team['id'] == constants.team_id:
                myteam.remain_life = team['remain_life']
            else:
                enemy.remain_life = team['remain_life']
        # logger.info(f"[R{round_id}]Myteam is in privilege mode")

        for player in players:
            if player['team'] == constants.team_id:
                # my player
                myteam.addPlayer(player, replace=False)
            else:
                enemy.addPlayer(player)
        # logger.debug(f"------{myteam.players}------")
        strategy.makeAction(action, round_id)

    
    # logger.info(f"=====\nround data {msg_data}\n {myteam.powers}=======")

    result = {
        "msg_name": "action",
        "msg_data": {
            "round_id": round_id,
            'actions': action
        }
    }
    return result


legInfo = []
def leg_end(msg):
    '''
    :param msg:
    {
        "msg_name" : "leg_end",
        "msg_data" : {
            "teams" : [
            {
                "id" : 1001,				#队ID
                "point" : 770             #本leg的各队所得点数
            },
            {
            "id" : 1002,
            "point" : 450
             }
            ]
        }
    }
    '''
    logger.info("leg end")
    teams = msg["msg_data"]['teams']
    for team in teams:
        legInfo.append("teams: %s,  point: %d" % (team['id'], team['point']))


def game_over(msg):
    logger.info(legInfo)
    logger.info("game over!")
