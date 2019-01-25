#ifndef WINUDPSOCKET_H
#define WINUDPSOCKET_H

#include <QByteArray>
#include <winsock2.h>

class CameraDataReceiver;

class WinUdpSocket
{
public:
	WinUdpSocket();
	virtual ~WinUdpSocket();
    bool bind(const char *host, int port);
    void startRead();

    void setReceiver(CameraDataReceiver *cdr);
    void setWorking(bool working);

    int getPacketCount() const;
    void clearPacketCount();

    int getByteRecved() const;
    void clearByteRecved();

private:
    int init();
    SOCKET clientSocket = INVALID_SOCKET;
    bool inited = false;
    CameraDataReceiver *cdr;
    int packetCount = 0;
    int byteRecved = 0;
    bool m_working = true;
    QByteArray recvedData;
    sockaddr fromaddr;

};
#endif

