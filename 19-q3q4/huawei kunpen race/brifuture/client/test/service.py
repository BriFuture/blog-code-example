#encoding:utf8
'''
测试
'''
import random

current_direction = ""


def exec_round(msg):
    self_balls = msg['msg_data']["self_balls"]
    current_round = msg['msg_data']['round_id']
    result = []

    for current in self_balls:
        result.append(
            {"ball_id": current['ball_id'], "force_x": random.randint(1, 10),
             "force_y": random.randint(1, 10)})

    result = {"msg_name": "action",
              "msg_data": {"round_id": current_round, "actions": result}}
    return result
