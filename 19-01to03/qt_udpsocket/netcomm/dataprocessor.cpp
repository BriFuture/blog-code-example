#include "dataprocessor.h"
#include <QDebug>
#include <QRgb>
#include <QPixmap>
#include <QThread>

const int DataProcessor::RawDataContainsMS = 500;
const int DataProcessor::LineDataContainsMS = 50;
const int DataProcessor::ImageWidth = 1500;
const int DataProcessor::ImageHeight = 150;

DataProcessor::DataProcessor(QObject *parent) : QObject(parent)
{
    processTimer.start();
}

DataProcessor::~DataProcessor()
{
    delete rawDataObjPool;
    delete lineDataPool;
}

void DataProcessor::init()
{
    image = QImage(ImageWidth, ImageHeight, QImage::Format_RGB32);
    rawDataObjPool = new RawDataObjPool(6);
    lineDataPool = new LineDataPool(30);
    datagramChecker = new QTimer(this);
    datagramChecker->setInterval(5000);
    datagramChecker->setSingleShot(false);
    connect(datagramChecker, &QTimer::timeout, [=] {
        checkDataMap();
    });
    datagramChecker->start();
}


void DataProcessor::processRawData(const QByteArray &rawdata)
{
    recvDatagramCount += 1;
    curData = rawDataObjPool->getAvailable();
    if( curData == 0 ) {
        rawDataObjPool->releaseAll();
        lossDatagramCount += 1;
        return;
    }
//    datagramChecker->start(); // restart timer
    curData->setRawData(rawdata);
    curData->setGenerateMs(processTimer.elapsed());
    processCameraData(curData);
    curData = 0;
}


void DataProcessor::processCameraData(CameraData *cd)
{
    int line = cd->getLineIdx();
    curProcessData = preProcessedMap.value(line, 0);

    if( curProcessData == 0 ) {
        curProcessData = lineDataPool->getAvailable();
        if( curProcessData == 0 ) {
            // 无法在现有的 map 中找到 PreProcessData， 又无法申请新的对象，需要检查是否需要释放内存
            lossDatagramCount += 1;
            checkDataMap();
            return;
        }
        curProcessData->setGenerateMs(processTimer.elapsed());
        curProcessData->setLine(line);
//        preProcessedMap.insert(line, curProcessData->getId());
        preProcessedMap[line] = curProcessData;
    }
    curProcessData->put(cd);
    rawDataObjPool->release(cd);
    curProcessData->isReady();
    // 某行的某个颜色通道数据完整
    if( curProcessData->isReady() ) {
        lineReadyCount += 1;
//        preProcessedMap.remove(line);
        processLineData(curProcessData);
    }
    curProcessData = 0;
}


void DataProcessor::checkDataMap(bool force)
{
    // 防止内存泄漏，一般每 6 行数据被接受到，map 就会清空一次，超过 12 说明有丢包
    int sizeBefore = preProcessedMap.size();
//      preProcessedMap.clear();
//      lineDataPool->releaseAll();
      int current = processTimer.elapsed();
    PreProcessData *ppd;
    QMutableMapIterator<int, PreProcessData*> it(preProcessedMap);
    while(it.hasNext()) {
        auto item = it.next();
        ppd = item.value();
        bool over = current - ppd->getGenerateMs() > LineDataContainsMS;
        if(over && !ppd->isReady()) {
            lineDataPool->release(ppd);
            it.remove();
        }
    }
    int sizeChanged = sizeBefore - preProcessedMap.size();
    clearMapCount += 1;
    if(clearMapCount % 10000 == 0)
        qDebug() << "Memory Leaked When Process Lines";
}


void DataProcessor::processLineData(PreProcessData *ppd)
{
    QRgb *line = reinterpret_cast<QRgb *>(image.scanLine(recvLineCount));
    const QByteArrayList &list = ppd->getDataList();

    for(int i = 0; i < 1280; i++) {
        line[i] = QColor(uchar(list[0][i]), uchar(list[1][i]), uchar(list[2][i])).rgb();
    }

    recvLineCount += 1;
    lineDataPool->release(ppd);
    preProcessedMap.remove(ppd->line());

    if( recvLineCount == ImageHeight - 1) {
        recvLineCount = 0;
//        qDebug() << "生成图片用时为 " << timer.elapsed() << "ms";
        imageReady(QPixmap::fromImage(image));
    }
}
