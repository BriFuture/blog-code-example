import socket
from .util import logger
import traceback
import json

SOCKET_CACHE = 1024 * 10

def add_str_len(msg_data, offset = 0):
    length = str(len(msg_data) + offset)
    index = 5 - len(length)
    if index < 0:
        raise Exception("the return msg data is too long. the length > 99999.")
    return '0' * index + length + msg_data

    
def remove_json_num(msg):
    return msg[5:]

class SockServer:

    def __init__(self, port: int):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.bind(("0.0.0.0", port))
        self.sock.listen(2)
        self.remainBytes = 0

    def accept(self):
        conn, addr = self.sock.accept()
        logger.debug(f"{addr} Connect")
        return conn, addr
    
    def recv(self, conn: socket.socket):
        """传入 Connection 对象，返回其接受到的内容 (dict)
        @note为了获得完整的数据，在此处添加一个循环
        """
        while 1:
            d = None
            try:
                d = conn.recv(SOCKET_CACHE)
                if d[-1] == 0:
                    d = d[:-1]
                d = d.decode()
                if self.remainBytes == 0:
                    if d[0:5].isdigit() and d[5] == "{":
                        need = int(d[0:5])
                        self.remainBytes = need - (len(d) - 5)
                        # logger.debug(f"remain: {self.remainBytes}, len: {len(d)}")
                        if(self.remainBytes <= 0): 
                            self._cache = ""
                            data = remove_json_num(d)
                            return json.loads(data)
                        else:
                            self._cache = d
                else:
                    self.remainBytes -= len(d)
                    if self.remainBytes == 0:
                        data = remove_json_num(self._cache + d)
                        logger.debug(self._cache + d)
                        self._cache  = ""
                        return json.loads(data)
                    else:
                        self._cache += d
            except ConnectionResetError as e:
                pass
            except Exception as e:
                logger.warning(f"[Receiver] receive data error. cach the data and wait for next. {e}")
                # logger.debug(d)
                traceback.print_exc()

def sendMsg(conn: socket.socket, msg: dict):
    return conn.send(add_str_len(json.dumps(msg)).encode())