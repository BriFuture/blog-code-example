#include "controller.h"
#include <QHostAddress>
#include <QDebug>

const int Controller::DataPort = 9000;
const int Controller::CONTROL_PORT = 10000;
const QStringList Controller::BOARD_IP = QStringList()
        << "192.168.8.31" << "192.168.8.32"
        << "192.168.8.33" << "192.168.8.34"
        << "192.168.8.35" << "192.168.8.36";

Controller::Controller(QObject *parent) : QObject(parent)
{

}

Controller::~Controller()
{
    qDebug() << "Controller is destroying";
}

void Controller::start()
{
    statsInterval = 5 * 1000;
    statsTimer = new QTimer(this);
    statsTimer->setSingleShot(false);
    connect(statsTimer, &QTimer::timeout, [=] {
        qDebug() << "FPS" << cdr->getPacketCount() / statsInterval
                 << "Size(M)" << cdr->getByteRecved() / 1024 / 1024;
        cdr->clearByteRecved();
        cdr->clearPacketCount();
    });
    statsTimer->start(statsInterval);

    recvProcessThread.start();
    recvThread.start();

    cdr = new CameraDataReceiver();
    cdr->moveToThread(&recvThread);

    dp = new DataProcessor();
    dp->moveToThread(&recvProcessThread);

    connect(cdr, &CameraDataReceiver::rawDataRecved, dp, &DataProcessor::processRawData);
    connect(&recvThread,        &QThread::finished, cdr, &CameraDataReceiver::deleteLater);
    connect(&recvProcessThread, &QThread::finished, dp, &DataProcessor::deleteLater);

    QMetaObject::invokeMethod(cdr, "initSocket");
    QMetaObject::invokeMethod(dp, "init");
}

void Controller::stop()
{
    disconnect(cdr, &CameraDataReceiver::rawDataRecved, dp, &DataProcessor::processRawData);
    cdr->stop();
    recvThread.wait(1000);
    recvThread.quit();
    if(!recvThread.wait(3000))
    {
        recvThread.terminate();
        recvThread.wait();
    }
    recvProcessThread.quit();
    if(!recvProcessThread.wait(3000)) //Wait until it actually has terminated (max. 3 sec)
    {
        recvProcessThread.terminate(); //Thread didn't exit in time, probably deadlocked, terminate it!
        recvProcessThread.wait(); //We have to wait again here!
    }
}

DataProcessor *Controller::getDataProcessor() const
{
    return dp;
}


