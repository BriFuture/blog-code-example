/**
 * For more help about UAC on windows, please refer:
 * https://www.devbean.net/2016/03/enable-qt-app-admin-on-win/
 * or https://www.cnblogs.com/findumars/p/5277434.html
*/


#include "registermodifier.h"
#include <QSettings>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QTextStream>


const int maxLatency = 3;
const QString registerPath = QString("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Enum\\FTDIBUS");
const QString concretePath = QString("\\0000\\Device Parameters\\LatencyTimer");
const QString latencyKey   = QString("LatencyTimer");
const QString nameKey      = QString("PortName");

RegisterModifier::RegisterModifier()
{

}

void RegisterModifier::setCom(const QString &com)
{
    this->com = com;
}

void RegisterModifier::modify()
{
    QFile file("./modifierLog.txt");
    file.open(QFile::WriteOnly);
    QTextStream ts(&file);

    QSettings set( registerPath, QSettings::NativeFormat );
    int latency = 0;
    bool needModify = false;
    foreach (QString device, set.allKeys()) {
        if( device.endsWith( latencyKey ) ) {
            latency = set.value( device ).toInt();
            if( latency > maxLatency ) {
                set.setValue( device, maxLatency );
                set.sync();
                latency = set.value( device ).toInt();
                ts << device << " ==> " << latency;
                qDebug() << device << " ==> " << latency;
                needModify = true;
            }
        }
    }
    if( !needModify ) {
        ts << "No need to modify.";
        qDebug() << "No need to modify.";
    }
    file.close();
}

