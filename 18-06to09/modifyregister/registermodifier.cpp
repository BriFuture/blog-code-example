/**
 * For more help about UAC on windows, please refer:
 * https://www.devbean.net/2016/03/enable-qt-app-admin-on-win/
 * or https://www.cnblogs.com/findumars/p/5277434.html
*/


#include "registermodifier.h"
#include <QDebug>
#include <QString>
#include <QFile>
#include <QTextStream>


const int RegisterModifier::defaultMaxLatency = 3;
const QString RegisterModifier::registerPath = QString("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Enum\\FTDIBUS");
const QString RegisterModifier::concretePath = QString("0000/Device Parameters");
const QString latencyKey   = QString("LatencyTimer");
const QString nameKey      = QString("PortName");

RegisterModifier::RegisterModifier()
{
    latency = defaultMaxLatency;
    set = new QSettings(registerPath, QSettings::NativeFormat);
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

    int currLatency = 0;
    bool needModify = false;
    readDevices();
    QMapIterator<QString, RegisterDevice> it(m_devices);
    while(it.hasNext()) {
        it.next();
        const RegisterDevice &rd = it.value();
        if(!com.isEmpty() && it.key() != com) {
            continue;
        }
        QString key = getKey(rd.id, latencyKey);
        currLatency = set->value(key).toInt();
        if(currLatency > latency) {
            QString info = QString("Set %1 latency, from %2 to %3").arg(it.key()).arg(currLatency).arg(latency);
            qInfo() << info;
            ts << info;
            set->setValue(key, latency);
            needModify = true;
        }
    }
    set->sync();

    if( !needModify ) {
        ts << "No need to modify.";
        qDebug() << "No need to modify.";
    }
    file.close();
}

void RegisterModifier::setLatency(int l) {
    if(l < 1) {
        l = 1;
    }
    latency = l;
}

int RegisterModifier::getLatency(const QString &com)
{
    readDevices();
    if(m_devices.contains(com)) {
        return m_devices[com].latency;
    }
    return 16;
}

QString RegisterModifier::getKey(const QString &device, const QString &item)
{
    return QString("%2/%3/%4").arg(device).arg(concretePath).arg(item);
}

void RegisterModifier::readDevices()
{
    int slash;
    RegisterDevice rd;
    foreach (QString device, set->allKeys()) {
        slash = device.indexOf("/");
        if(slash > 0) {
            QString name = device.left(slash);
            QString portname = set->value(getKey(name, nameKey)).toString();
            int latency = set->value(getKey(name, latencyKey)).toInt();
            if(m_devices.contains(portname)) {
                continue;
            }
            rd.id = name;
            rd.latency = latency;
//            qDebug() << name << portname << getKey(name, nameKey);
            m_devices.insert(portname, rd);
        }
    }
}

