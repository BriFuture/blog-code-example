#include <QCoreApplication>

#include "instructparser.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    InstructParser ip;
    ip.parse( "test.xml" );

    return a.exec();
}
