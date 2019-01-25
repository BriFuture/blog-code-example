#ifndef VIDEODISPLAYER_H
#define VIDEODISPLAYER_H

#include <QWidget>
#include <QPixmap>
#include <QImage>
#include <QPainter>


#include "cameradata.h"

namespace Ui {
class VideoDisplayer;
}

class VideoDisplayer : public QWidget
{
    Q_OBJECT

public:
    explicit VideoDisplayer(QWidget *parent = 0);
    ~VideoDisplayer();
    static const int ImageWidth;
    static const int ImageHeight;

public slots:
    void onImageReady(const QPixmap &image);

private:
    Ui::VideoDisplayer *ui;
    QPixmap pixmap;
    QImage image;
    QPainter *painter;
    int recvLineCnt = 0;
};

#endif // VIDEODISPLAYER_H
