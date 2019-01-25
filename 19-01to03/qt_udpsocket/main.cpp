#include "mainwindow.h"
#include <QApplication>
#include "backend.h"
#include "videodisplayer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    VideoDisplayer vd;
    w.setCentralWidget(&vd);
    Backend *be = Backend::instance();
    QObject::connect(&a, &QApplication::aboutToQuit, be, &QObject::deleteLater);
    QObject::connect(be->control->dp, &DataProcessor::imageReady, &vd, &VideoDisplayer::onImageReady);

    return a.exec();
}
