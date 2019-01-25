#include "videodisplayer.h"
#include "ui_videodisplayer.h"
#include <QDebug>
#include <QTime>

const int VideoDisplayer::ImageWidth = 1500;
const int VideoDisplayer::ImageHeight = 150;

VideoDisplayer::VideoDisplayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoDisplayer)
{
    ui->setupUi(this);
//    pixmap = QPixmap(1500, 30);
//    pixmap.fill(Qt::white);
    image = QImage(ImageWidth, ImageHeight, QImage::Format_RGB32);
    ui->label->setPixmap(QPixmap::fromImage(image));
}

VideoDisplayer::~VideoDisplayer()
{
    delete ui;
}

void VideoDisplayer::onImageReady(const QPixmap &image)
{
    ui->label->setPixmap(image);
}

