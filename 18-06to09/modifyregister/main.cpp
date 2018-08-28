#include <QCoreApplication>
#include "registermodifier.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    RegisterModifier rm;
    rm.modify();
//    return a.exec();
    return 0;
}
