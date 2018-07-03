#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

#pragma execution_character_set("utf-8")

#define debugmode

#ifdef debugmode
QtMsgType msgLevel = QtDebugMsg;
#elif
QtMsgType msgLevel = QtWarningMsg;
#endif


class Log {

public:
    static void setLogFile(const QString &name)
    {
        file.setFileName( name );
//        file.open( QIODevice::WriteOnly | QIODevice::Append );
        file.open( QIODevice::WriteOnly );
        outstream.setDevice( &file );
        outstream.setCodec( QTextCodec::codecForName("UTF-8") );
    }

    static void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
    {
        QString formatMsg = qFormatLogMessage( type, context, msg );
        if( type < msgLevel ) {
            return;
        }
        outstream << formatMsg << "\r\n";
        outstream.flush();
        std::cout << formatMsg.toLocal8Bit().constData() << std::endl;

//        QByteArray localMsg = msg.toLocal8Bit();
//        switch (type) {
//        case QtDebugMsg:
//            fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
//            break;
//        case QtInfoMsg:
//            fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
//            break;
//        case QtWarningMsg:
//            fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
//            break;
//        case QtCriticalMsg:
//            fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
//            break;
//        case QtFatalMsg:
//            fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
//            break;
//        }
    }
private:
    static QFile file;
    static QTextStream outstream;
};

QFile Log::file;
QTextStream Log::outstream;


// entry
int main(int argc, char *argv[])
{
    qInstallMessageHandler( Log::myMessageOutput );
    QCoreApplication a(argc, argv);

    QString pattern;
#ifdef debugmode
    pattern = QString( "[%{time yyyyMMdd h:mm:ss.zzz t}")
            + QString( " %{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}]" )
            + QString( " %{file}:%{function}:%{line} -- %{message}" );
#else
    pattern = QString( "[%{time yyyyMMdd h:mm:ss.zzz t}")
            + QString( " %{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}]" )
            + QString( " -- %{message}" );
#endif

    qSetMessagePattern( pattern );

    Log::setLogFile( "log.txt" );

//    std::cout << obj.objectName().toLocal8Bit().constData();  // 显示到控制台（不是Creator集成的控制台）
    QObject dobj;
    dobj.setObjectName( "Debug Test" );
    qDebug() << dobj.objectName();
    QObject wobj;
    wobj.setObjectName( "Warning Test" );
    qWarning() << wobj.objectName();
    QObject cobj;
    cobj.setObjectName( "Critical Test" );
    qCritical() << cobj.objectName();
    QObject iobj;
    iobj.setObjectName( "Info Test" );
    qInfo() << iobj.objectName();
//    QObject fobj;
//    fobj.setObjectName( "Fatal Test" );
//    qFatal(fobj.objectName().toLocal8Bit().constData());
    return a.exec();
}
