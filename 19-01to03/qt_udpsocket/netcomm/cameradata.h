#ifndef CAMERADATA_H
#define CAMERADATA_H

#include <QObject>

class DataObj : public QObject
{
    Q_OBJECT
public:
    explicit DataObj(QObject *parent = 0) : QObject(parent) {}
    int getId() const;
    void setId(int value);

    bool isValid() const;
    void setValid(bool value);

    int getGenerateMs() const;
    void setGenerateMs(int value);

    virtual void reset();

protected:
    int id = -1;
    bool valid = true;
    int generateMs = -1;

};

/**
 * @brief The CameraData class
 * 对应从下位机接收到的字节数组的类，原始数据包，需要经过处理后变成一行数据
 */
class CameraData : public DataObj
{
    Q_OBJECT
public:
    enum RGBType {
        R = 1,
        G = 2,
        B = 3,
        UNKOWN = 0
    };
    static const QByteArray DATA_START_MAGIC;
    static const QByteArray DATA_END_MAGIC;
    static const int PacketSize;

    explicit CameraData(QObject *parent = 0);
    ~CameraData();

    bool isPackageValid();
    // 获取保留区域的数据
    QByteArray getReserved();
    // 设置原始数据
    void setRawData(const QByteArray &value);
    void setRawData(const char *data);
    // 获取数据区域内的所有数据，默认获取有效数据
    QByteArray getData(bool valid = true);
    int getPackageCntInLine();
    int getPackageIdxInLine();
    int getSampleDiffLine();
    int getRGBExtern();
    RGBType getRGBType();
    int getLineIdx();
    int getValidDataLen();
    int getLineBytes();
    int sliceToInt(int start, int len = 4);

    // DataObj interface
    void reset();

signals:

public slots:

private:
    inline QByteArray slice(int start, int len = -1);
    inline QByteArray getStartMagic();
    inline QByteArray getEndMagic();
    QByteArray data;
    int packageCntInLine = -1;
    int packegeIdxInLine = -1;
    int lineIdx = -1;
    int lineBytes = -1;
    int rgbType = -1;

};




/**
 * @brief The PreProcessData class
 * 预处理数据
 */
class PreProcessData: public DataObj
{
    Q_OBJECT

public:
    static const int PacketSize;
    static const int PacketPerLine;
    explicit PreProcessData(QObject *parent = 0, int line = -1);
    void put(CameraData *cd);

    bool isReady();
    void reset();

    int line() const;
    void setLine(int line);

    const QByteArrayList &getDataList() const;
    QByteArray repr();

private:
    /**
     * @brief cameraData
     * 每 2 个 CameraData 构成一行的单通道数据，有序存放 RGB 通道数据
     * 0-1 存放 R，2-3 存放 G， 4-5 存放 B
     */
    QByteArrayList dataList;
    int m_line;
    int m_readyCount = 0;
    int m_duplicateCount = 0;
    bool *dataPlaced = 0;
//    bool dataPlaced[6];
};

#endif // CAMERADATA_H
