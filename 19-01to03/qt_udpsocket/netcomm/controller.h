#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QThread>
#include <QTimer>

#include "dataprocessor.h"
#include "cameradatareceiver.h"

/*
#include <QJsonObject>

class Command: public QObject
{
    Q_OBJECT
public:
    explicit Command(QObject *parent = 0);
signals:

public slots:

private:
    void genCamera(const QString &id);
    void genBoard(const QString &id);

};
*/

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);
    ~Controller();
    static const int DataPort;
    static const int CONTROL_PORT;
    static const QStringList BOARD_IP;

    void start();
    void stop();


    DataProcessor *getDataProcessor() const;

signals:

public slots:
private:
    CameraDataReceiver *cdr;
    QThread recvThread;
    QThread recvProcessThread;
    QByteArrayList rawdataList;
    DataProcessor *dp = 0;

    QTimer *statsTimer;
    int statsInterval;
};

#endif // CONTROLLER_H
