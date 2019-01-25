#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <QObject>
#include <QMap>
#include <QHash>
#include <QList>
#include <QImage>
#include <QTime>
#include <QTimer>
#include <QColor>
#include "cameradata.h"
#include "cameradatapool.h"

class LineDataProcessor;

class DataProcessor: public QObject
{
    Q_OBJECT
public:
    static const int RawDataContainsMS;
    static const int LineDataContainsMS;
    static const int ImageWidth;
    static const int ImageHeight;

    explicit DataProcessor(QObject *parent = 0);
    ~DataProcessor();

public slots:
    void init();
    /**
     * @brief processRawData
     * @param rawdata
     * 处理来自底层的原始数据，每个原始数据是一个数据包
     */
    void processRawData(const QByteArray &rawdata);

signals:
    void datagramStats(int recv, int loss);
    void imageReady(const QPixmap &pixmap);

private slots:
    void checkDataMap(bool force = false);
    void processLineData(PreProcessData *ppd);

private:
    /*
    int countRawLinesAt(int line, const CameraData::RGBType rgbType, int max = 3);
     * @brief genRawDataIndex
     * @param line
     * @param rgb
     * @param max
     * @return
     * 为 每一个原始数据包生成一个唯一的 index
    quint64 genRawDataIndex(int line, CameraData::RGBType rgb, int max = -1);
    QMap<quint64, CameraData *> rawLines;  // 至少 6 个 CameraData 构成一行数据
    QMap<int, ProcessedLineData *> processedLinesMap;
    */
    void processCameraData(CameraData *cd);

    QMap<int, PreProcessData *> preProcessedMap;
    QImage image;
    QTime processTimer;
    QTimer *datagramChecker;
    RawDataObjPool *rawDataObjPool;
    LineDataPool *lineDataPool;
    CameraData *curData = 0;
    PreProcessData *curProcessData = 0;

    int recvDatagramCount = 0;
    int lossDatagramCount = 0;
    int lineReadyCount = 0;
    int recvLineCount = 0;
    int clearMapCount = 0;

    friend class LineDataProcessor;
};

#endif // DATAPROCESSOR_H
