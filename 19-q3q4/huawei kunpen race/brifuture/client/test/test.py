a = '{"msg_name": "action", "msg_data": {"round_id": 2, "action": [{"player_id": 0, "move": ["up"], "team": 1}]}}'

import json

import ballclient.comunicate.client as client

print client.add_str_len(a)

a = '''{"msg_name": "action", "msg_data": {"round_id": 2, "action": [
    {"player_id": 0, "move": ["up"], "team": 1}]}}'''


print type(json.loads(a))

