# -*- coding: utf-8 -*-

# from gevent import monkey
# monkey.patch_all()
# from gevent import sleep

DATA_PORT = 9000

DATA_START_MAGIC = bytearray(4)
DATA_START_MAGIC[0] = 0x53  # S
DATA_START_MAGIC[1] = 0x74  # t
DATA_START_MAGIC[2] = 0x61  # a
DATA_START_MAGIC[3] = 0x72  # r

DATA_END_MAGIC = bytearray(4)
DATA_END_MAGIC[0] = 0x54 # T
DATA_END_MAGIC[1] = 0x45 # E
DATA_END_MAGIC[2] = 0x6e # n
DATA_END_MAGIC[3] = 0x64 # d

slice_start_magic = slice(0, 4)
slice_partial_cnt =  4
slice_partial_idx =  5
slice_sample_line =  6
slice_rgb_extern =  7
slice_line_idx = slice(8, 12)
slice_valid_data_len = slice(12, 16)
slice_line_bytes = slice(16, 20)
slice_resv = slice(20, 148)
slice_data = slice(148, 1428)
slice_end_magic = slice(1428, 1432)

import numpy as np
class CameraData(object):
    def __init__(self):
        # self.new()
        # self.rawdata = rawdata
        self.dataLow = 10
        self.dataHigh = 20
        self.new()

    def genRandomByte(self, by=4):
        r = bytearray(by)
        for i in range(by):
            r[i] = np.random.randint(0, 255)

    def setPackageIdx(self, i = 0):
        self.rawdata[slice_partial_idx] = i

    def setRGB(self, c = 1):
        self.rawdata[slice_rgb_extern] = c

    def setLineIdx(self, line):
        start = slice_line_idx.start
        self.rawdata[start+3] = 0x000000ff & line
        self.rawdata[start+2] = (0x0000ff00 & line) >> 8
        self.rawdata[start+1] = (0x00ff0000 & line) >> 16
        self.rawdata[start+0] = (0xff000000 & line) >> 24

    def setValidDataLen(self, len):
        start = slice_valid_data_len.start
        self.rawdata[start+3] = 0x000000ff & len
        self.rawdata[start+2] = (0x0000ff00 & len) >> 8
        self.rawdata[start+1] = (0x00ff0000 & len) >> 16
        self.rawdata[start+0] = (0xff000000 & len) >> 24

    def setLineBytes(self, len):
        start = slice_line_bytes.start
        self.rawdata[start+3] = 0x000000ff & len
        self.rawdata[start+2] = (0x0000ff00 & len) >> 8
        self.rawdata[start+1] = (0x00ff0000 & len) >> 16
        self.rawdata[start+0] = (0xff000000 & len) >> 24

    def randomData(self):
        size = slice_data.stop - slice_data.start
        arr = np.random.randint(self.dataLow, self.dataHigh, size, dtype=np.uint8)
        self.rawdata[slice_data] = bytearray(arr)

    def new(self):
        """构造新的数据对象
        """
        self.rawdata = bytearray(1432)
        self.rawdata[slice_start_magic] = DATA_START_MAGIC
        self.rawdata[slice_partial_cnt] = 0x02
        self.rawdata[slice_partial_idx] = 0x00
        self.rawdata[slice_sample_line] = 0x03
        self.rawdata[slice_rgb_extern] = 0x01

        self.setLineIdx(0x00)
        self.setValidDataLen(1280)
        self.setLineBytes(1432)

        # for i in range(slice_resv.start, slice_resv.stop):
        #     self.rawdata[i] = 0xee

        self.randomData()
        self.rawdata[slice_end_magic] = DATA_END_MAGIC

    def hex(self):
        return self.rawdata.hex()

    def __repr__(self):
        return '<CameraData@{} hex len: {}>'.format(hex(id(self)), len(self.rawdata))

# from gevent import socket
import socket
class UdpServer( object ):
    """该类功能是处理底层的 UDP 数据包发送和接收，利用队列缓存所有数据
    """

    def __init__(self, *args, **kwargs):
        self._sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self._sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        # self._sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
        self._sock.bind( ('', DATA_PORT+11 ) )
        self._sock.settimeout( None )  # never timeout
        # self._sock.setblocking( 0 ) # none block


    def send_msg( self, msg ):
        """发送消息，
        @param msg 字典对象，发送 msg 的 rawdata 字段
        """
        self._sock.sendto( msg.rawdata, ('192.168.8.1', DATA_PORT))

artifacialData = 0
def randomMsgData(msg):
    global artifacialData
    msg.randomData()
    meandata = int(msg.dataLow / 2) + int(msg.dataHigh / 2)
    
    if artifacialData == 0:
        meandata += np.random.randint(-2, 5, dtype=np.int8)
    else:
        meandata += np.random.randint(-5, 2, dtype=np.int8)

    if meandata < 15:
        meandata = 15
        artifacialData = 0
    elif meandata > 240:
        meandata = 240
        artifacialData = 1
    msg.dataLow = np.random.randint(meandata - 10, meandata - 5, dtype=np.uint8)
    msg.dataHigh = np.random.randint(meandata + 5, meandata + 10, dtype=np.uint8)
    
from time import sleep
from gevent import spawn, joinall
import cv2 as cv
class DataSender(object):

    def __init__(self):
        self.server = UdpServer()
        self.msgs = []
        # self.server.append(UdpServer())
        for i in range(3):
            # self.server.append(UdpServer())
            msg = CameraData()
            msg.setValidDataLen(1280)
            msg.setRGB(i+1)
            self.msgs.append(msg)

    def serve(self):
        lineNumber = 150 * 60
        line = 0
        while True:
            e1 = cv.getTickCount()
            for x in range(lineNumber):   
                self.send(0)
                self.send(1)
                if line % 10 != 0:
                    self.send(2)
                # joinall([
                #     spawn(self.sendR),
                #     spawn(self.sendG),
                #     spawn(self.sendB),
                # ])
                line += 1
                for i in range(3):
                    self.msgs[i].setLineIdx(line)

                # mock data, not simple random
                if x %  150 == 0:
                    for i in range(3):
                        randomMsgData(self.msgs[i])
                    sleep(0)

            e2 = cv.getTickCount()
            elapsed = ( e2 - e1 ) / cv.getTickFrequency()
            print("FPS: %.2f Used time: %.4fms, line: %d " 
                % (lineNumber / elapsed / 150, elapsed, line ))
            print("msg data range: {} - {}, msg data range: {} - {}, msg data range: {} - {}, "\
                .format(self.msgs[0].dataLow, self.msgs[0].dataHigh, 
                    self.msgs[1].dataLow, self.msgs[1].dataHigh,
                    self.msgs[2].dataLow, self.msgs[2].dataHigh)
                )

    def send(self, i = 0):
        self.msgs[i].setPackageIdx(0)
        self.server.send_msg(self.msgs[i])
        sleep(0)
        self.msgs[i].setPackageIdx(1)
        self.server.send_msg(self.msgs[i])
        sleep(0)

    def sendR(self):
        self.send(0)

    def sendG(self):
        self.send(1)

    def sendB(self):
        self.send(2)

class RGBSender(object):
    def __init__(self, irgb):
        self.server = UdpServer()
        self.msg = CameraData()
        self.msg.setValidDataLen(1280)
        self.msg.setRGB(irgb+1)
        self.rgb = irgb+1
        print("RGBSender is initialized", irgb)

    def send(self):
        self.msg.setPackageIdx(0)
        self.server.send_msg(self.msg)
        sleep(0)
        self.msg.setPackageIdx(1)
        self.server.send_msg(self.msg)
        sleep(0)

    def serve(self):
        lineNumber = 150 * 60
        line = 0
        print("RGBSender is about to work")
        while True:
            e1 = cv.getTickCount()
            for x in range(lineNumber):   
                self.send()
                line += 1
                self.msg.setLineIdx(line)

                # mock data, not simple random
                if x %  150 == 0:
                    randomMsgData(self.msg)
                    sleep(0)

            e2 = cv.getTickCount()
            elapsed = ( e2 - e1 ) / cv.getTickFrequency()
            print("RGB: %d FPS: %.2f Used time: %.4fms, line: %d " 
                % (self.rgb, lineNumber / elapsed / 150, elapsed, line ))
            print("msg data range: {} - {}"\
                .format(self.msg.dataLow, self.msg.dataHigh)
                )
        print("RGBSender is done")

def singleMain():
    from multiprocessing import Process
    try:
        dataSender = DataSender()
        # dataSender.serve()
        p = Process(target=dataSender.serve)
        p.start()
    except Exception as e:
        print("main", e)

def multiSend():
    rSender = RGBSender(0)
    gSender = RGBSender(1)
    bSender = RGBSender(2)
    # from multiprocessing import pool
    # p = pool.Pool(3)
    from multiprocessing import Process
    try:
        rp = Process(target=rSender.serve)
        gp = Process(target=gSender.serve)
        bp = Process(target=bSender.serve)
        # p.apply_async(func=rSender.serve)
        # p.apply_async(func=gSender.serve)
        # p.apply_async(func=bSender.serve)
        print("Start Sending")
        rp.start()
        gp.start()
        bp.start()
        # p.close()
        # p.join()
        print("Send Over")
    except Exception as e:
        print(e)

if __name__ == '__main__':
    singleMain()
    # multiSend()
