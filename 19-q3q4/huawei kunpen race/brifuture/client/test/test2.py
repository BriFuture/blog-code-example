import ballclient.comunicate.client as client
import json

a = '{"msg_name": "action", "msg_data": {"round_id": 659, "actions": [{"force_y": 8, "force_x": 10, "ball_id": 0}, {"force_y": 10, "force_x": 9, "ball_id": 1}, {"force_y": 5, "force_x": 1, "ball_id": 2}, {"force_y": 2, "force_x": 5, "ball_id": 3}]}}'
a ='''{"msg_name": "round",
 "msg_data": {
     "round_id": 2,
     "players": [
         {
             "id": 0, "team": 1, "x": 0, "y": 1, sleep: 1, score: 20
         }
 }
}'''

b = json.dumps(a)

print client.add_str_len(b)
