#ifndef REGISTERMODIFIER_H
#define REGISTERMODIFIER_H

#include <QString>
#include <QStringList>
#include <QSettings>
#include <QMap>

class RegisterModifier
{
public:
    static const int defaultMaxLatency;
    static const QString registerPath;
    static const QString concretePath;
    static QString getKey(const QString &device, const QString &item);
    struct RegisterDevice {
        QString id;
        int latency;
    };

    RegisterModifier();
    void setCom(const QString &com);
    void modify();

//    QString

    void setLatency(int l);

    int getLatency(const QString &com);

protected:
    void readDevices();
    QString com;
    int latency = 0;
    QMap<QString, RegisterDevice> m_devices;

    QSettings *set;
};

#endif // REGISTERMODIFIER_H
