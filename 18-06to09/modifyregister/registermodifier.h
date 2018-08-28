#ifndef REGISTERMODIFIER_H
#define REGISTERMODIFIER_H

#include <QString>

class RegisterModifier
{
public:
    RegisterModifier();
    void setCom(const QString &com);
    void modify();

protected:
    QString com;
};

#endif // REGISTERMODIFIER_H
