#include "backend.h"
#include <QDebug>

static Backend *backend;

Backend *Backend::instance()
{
    if(backend == 0) {
        backend = new Backend();
    }
    return backend;
}

Backend::~Backend()
{
    control->stop();
    delete control;
}


Backend::Backend(QObject *parent) : QObject(parent)
{
    control = new Controller();
    control->start();
}
