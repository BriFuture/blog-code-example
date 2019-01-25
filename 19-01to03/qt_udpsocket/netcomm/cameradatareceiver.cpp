#include "cameradatareceiver.h"
#include "controller.h"

CameraDataReceiver::CameraDataReceiver(Controller *parent) : QObject(parent)
{
}

CameraDataReceiver::~CameraDataReceiver()
{
    qDebug() << "CameraDataReceiver destroying";
    delete winUdpSocket;
}


void CameraDataReceiver::initSocket()
{
    winUdpSocket = new WinUdpSocket();
//    winUdpSocket->setReceiver(this);
//    winUdpSocket->bind("192.168.8.1", Controller::DataPort);
//    winUdpSocket->startRead();

    udpSocket = new QUdpSocket(this);
    udpSocket->setReadBufferSize(50*1024*1024);
    udpSocket->bind(QHostAddress("192.168.8.1"), Controller::DataPort);
    connect(udpSocket, &QUdpSocket::readyRead,
                this, &CameraDataReceiver::readDatagrams);
}

void CameraDataReceiver::stop()
{
    winUdpSocket->setWorking(false);
}

void CameraDataReceiver::readDatagrams()
{
    while(udpSocket->hasPendingDatagrams()) {
        int size = udpSocket->pendingDatagramSize();
        datagram.resize(size);
        packetCount += 1;
        byteRecved += size;
        udpSocket->readDatagram(datagram.data(), size);
        rawDataRecved(datagram);
//        storeRawData(datagram);
    }

}

int CameraDataReceiver::getByteRecved() const
{
    return byteRecved;
//    return winUdpSocket->getByteRecved();
}

void CameraDataReceiver::clearByteRecved()
{
    byteRecved = 0;
    winUdpSocket->clearByteRecved();
}

int CameraDataReceiver::getPacketCount() const
{
    return packetCount;
//    return winUdpSocket->getPacketCount();
}

void CameraDataReceiver::clearPacketCount()
{
    packetCount = 0;
    winUdpSocket->clearPacketCount();
}
