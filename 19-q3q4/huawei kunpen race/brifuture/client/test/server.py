# encoding:utf8
import socket
import threading

HOST = "127.0.0.1"
PORT = 3000

if __name__ == "__main__":
    _socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    _socket.bind((HOST, PORT))
    _socket.listen(5)
    while 1:
        conn, addr = _socket.accept()  # 接受TCP连接，并返回新的套接字与IP地址
        print('Connected by', addr)  # 输出客户端的IP地址
        data = conn.recv(1024)  # 把接收的数据实例化
        conn.sendall('00000{"msg_name":"leg_start","b":2}')
        threading._sleep(0.2)
        while 1:
            conn.sendall('''12345{"msg_name":"round","msg_data":{"round_id":2,"players":[{"id":0,"team":1,"x":0,"y":1, "sleep":1,"score":20}]}}''');
            threading._sleep(0.2)
            print "start receive"
            data = conn.recv(1024)
            print "receive:%s" % data
            data = data[5:]
            import json

            m = json.loads(data)
            print "*****************************"
            print m
            print "*****************************"
conn.close()  # 关闭连接
