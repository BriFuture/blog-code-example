# encoding:utf8
'''
客户端对数据的接受和对已生成数据的发送，选手可以不关注此模块

'''

import socket
import json

from ballclient.service import service
import ballclient.service.constants as constants
import traceback
from ballclient.util import logger
from time import sleep

_socket = None

SOCKET_CACHE = 1024 * 20


def try_again(func):
    def wraper(*args, **kwargs):
        connect_time = 1
        while connect_time <= 300:
            try:
                return func(*args, **kwargs)
            except Exception:
                if connect_time % 10 == 0:
                    logger.warning("connect server failed.....connect_time:%s" % connect_time)
                connect_time += 1
                sleep(0.2)
        logger.fatal("can not connect with server. %s,%s" % args)
        exit(1)
    return wraper

@try_again
def connect_socket(ip=None, port=None):
    global _socket
    _socket.connect((ip, port))


def start(ip=None, port=None):
    global _socket
    _socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    data = None
    try:
        connect_socket(ip,port)

        logger.debug("Tri to register.")
        register()
        while 1:
            data = _receive()
            if data is None:
                break
            # logger.debug(data)

            if data['msg_name'] == "round":
                message = service.round(data)
                send_dict(message)
            elif data['msg_name'] == "leg_start":
                service.leg_start(data)
            elif data['msg_name'] == "leg_end":
                service.leg_end(data)
            elif data['msg_name'] == "game_over":
                service.game_over(data)
                logger.info("Server Exit")
                return
            else:
                print("invalid msg_name.")
    except socket.error:
        print("can not connect with server. %s,%s" % (ip, port))
        exit()
    except Exception as e:
        print("[start] some error happend: %s. the receive data: %s" % (e, data) )
        traceback.print_exc()
    finally:
        if _socket:
            _socket.close()
        logger.info("Client Closed")


def register():
    '''
    register logic
    :return:
    '''
    data = {"msg_name": "registration",
            "msg_data": {"team_name": constants.team_name,
                         "team_id": constants.team_id}}
    # print(f"[Register] {data}")
    send_dict(data)


def send_dict(data):
    data_str = json.dumps(data)
    _socket.sendall(add_str_len(data_str).encode())


class Receiver(object):
    def __init__(self):
        self._cache = ""
        self.remainBytes = 0

    def __call__(self, *args, **kwargs):
        while 1:
            d = None
            try:
                d = _socket.recv(SOCKET_CACHE).decode()
                # logger.debug(f"Data: {d}, {len(d)}")
                if len(d) == 0:
                    return None
                if self.remainBytes == 0:

                    if d[0:5].isdigit() and d[5] == "{":
                        need = int(d[0:5])
                        self.remainBytes = need - (len(d) - 5)
                        if(self.remainBytes == 0): 
                            self._cache = ""
                            data = remove_json_num(d)
                            return json.loads(data)
                        else:
                            self._cache = d
                else:
                    self.remainBytes -= len(d)
                    if self.remainBytes == 0:
                        data = remove_json_num(self._cache + d)
                        # logger.debug(self._cache + d)
                        self._cache  = ""
                        return json.loads(data)
                    else:
                        self._cache += d
            except ConnectionResetError:
                print("can not connect with server.")
                exit()
            except Exception as e:
                print("[Receiver] receive data error. cach the data and wait for next.", e)
                traceback.print_exc()
                break


_receive = Receiver()

def add_str_len(msg_data):
    length = str(len(msg_data))
    index = 5 - len(length)
    if index < 0:
        raise Exception("the return msg data is too long. the length > 99999.")
    return '0' * index + length + msg_data


def remove_json_num(msg):
    return msg[5:]


"""
# def _receive():
#     d = _socket.recv(SOCKE_CACH)
#     data = remove_json_num(d)
#     return json.loads(data)


def receive_game_data():
    try:
        data = _receive()
        if data['msg_name'] == "game_over":
            service.exec_game_over()
            return 0
        else:
            return_msg = service.exec_round(data)
            send_msg = add_str_len(json.dumps(return_msg))
            _socket.sendall(send_msg)
    except:
        print('error receive data......')

    return 1
"""
