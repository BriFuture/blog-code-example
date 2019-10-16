#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include "registermodifier.h"
#include <iostream>

#include <QDebug>

#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("Com Property Modifier");
    QCoreApplication::setApplicationVersion("1.0.0");

    QCommandLineParser parser;
//    parser.setOptionsAfterPositionalArgumentsMode(QCommandLineParser::ParseAsOptions);
//    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    parser.setApplicationDescription("Used for modify com delay.");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption comOption({"c", "com"}, "specify com name (COM4 for example)", "portname");
    parser.addOption(comOption);

    QCommandLineOption delayOption({"d", "delay"}, "the delay value needs to be set.", "delay");
    parser.addOption(delayOption);

    QCommandLineOption deviceOption({{"D", "devices"}, "Read port name and latency of All Devices."});
    parser.addOption(deviceOption);

    parser.process(a);


    RegisterModifier rm;
    if(parser.isSet(deviceOption)) {
        int delay = rm.getLatency(parser.value(comOption));
        std::cout << delay << std::endl;
        return 0;
    }

    if(parser.isSet(delayOption)) {
        rm.setLatency(parser.value(delayOption).toInt());
    }

    if(parser.isSet(comOption)) {
        rm.setCom(parser.value(comOption));
        rm.modify();
    }

    return 0;
}
