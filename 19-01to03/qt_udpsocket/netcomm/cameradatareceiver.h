#ifndef CAMERADATARECEIVER_H
#define CAMERADATARECEIVER_H

#include <QUdpSocket>
#include <QObject>
#include <QTimer>
#include "WinUdpSocket.h"

class Controller;

class CameraDataReceiver: public QObject
{
    Q_OBJECT
public:
    explicit CameraDataReceiver(Controller *parent = 0);
    virtual ~CameraDataReceiver();

    int getPacketCount() const;
    void clearPacketCount();

    int getByteRecved() const;
    void clearByteRecved();

signals:
    void rawDataRecved(const QByteArray &rawData);
    void recvCharData(const char *data);

public slots:
    void initSocket();
    void stop();

private slots:
    void readDatagrams();
private:
    QUdpSocket *udpSocket;
    WinUdpSocket *winUdpSocket;
    QByteArray datagram;

    int packetCount = 0;
    int byteRecved = 0;

    friend class Controller;
};

#endif // CAMERADATARECEIVER_H
