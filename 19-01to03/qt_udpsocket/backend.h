#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include "controller.h"

/**
 * @brief The Backend class
 * 负责收集和运行后端程序
 */
class Backend : public QObject
{
    Q_OBJECT
public:
    Controller *control;
    static Backend* instance();
    ~Backend();

signals:

public slots:

private:
    explicit Backend(QObject *parent = 0);
};

#endif // BACKEND_H
