#include "cameradata.h"

#include <QDebug>


int DataObj::getId() const
{
    return id;
}

void DataObj::setId(int value)
{
    id = value;
}

bool DataObj::isValid() const
{
    return valid;
}

void DataObj::setValid(bool value)
{
    valid = value;
}

int DataObj::getGenerateMs() const
{
    return generateMs;
}

void DataObj::setGenerateMs(int value)
{
    generateMs = value;
}

void DataObj::reset()
{
    valid = true;
    generateMs = -1;
}

// =========================


const QByteArray CameraData::DATA_START_MAGIC = QByteArray("\x53\x74\x61\x72");
const QByteArray CameraData::DATA_END_MAGIC   = QByteArray("\x54\x45\x6e\x64");
const int CameraData::PacketSize = 1432;

const int START_MAGIC_OFF = 0;
const int START_MAGIC_LEN = 4;

const int PARTIAL_CNT_OFF = 4;
const int PARTIAL_CNT_LEN = 1;

const int PARTIAL_IDX_OFF = 5;
const int PARTIAL_IDX_LEN = 1;

const int SAMPLE_LINE_OFF = 6;
const int SAMPLE_LINE_LEN = 1;

const int RGB_EXTERN_OFF = 7;
const int RGB_EXTERN_LEN = 1;

const int LINE_IDX_OFF = 8;
const int LINE_IDX_LEN = 4;

const int VALID_DATALEN_OFF = 12;
const int VALID_DATALEN_LEN = 4;

const int LINE_BYTES_OFF = 16;
const int LINE_BYTES_LEN = 4;

const int RESERVED_OFF = 20;
const int RESERVED_LEN = 128;

const int VALID_DATA_OFF = 148;
const int VALID_DATA_LEN = 1280;

const int END_MAGIC_OFF = 1428;
const int END_MAGIC_LEN = 4;

CameraData::CameraData(QObject *parent) : DataObj(parent)
{
    data.resize(PacketSize);
}

void CameraData::setRawData(const QByteArray &value)
{
    data = value;
//    memcpy(data.data(), value, value.size());
    getPackageIdxInLine();
    getLineIdx();
    getRGBExtern();
}

void CameraData::setRawData(const char *data)
{
    memcpy(this->data.data(), data, PacketSize);
}

CameraData::~CameraData()
{
}

bool CameraData::isPackageValid()
{
    auto start = (this->getStartMagic() == this->DATA_START_MAGIC);
    auto end = (this->getEndMagic() == this->DATA_END_MAGIC);
    return start && end;
}


QByteArray CameraData::getReserved()
{
    return slice(RESERVED_OFF, RESERVED_LEN);
}

QByteArray CameraData::getData(bool valid)
{
    if(valid) {
        int len = this->getValidDataLen();
        return slice(VALID_DATA_OFF, len);
    }
    return slice(VALID_DATA_OFF, VALID_DATA_LEN);
}

inline QByteArray CameraData::getStartMagic()
{
    return slice(START_MAGIC_OFF, START_MAGIC_LEN);
}

inline QByteArray CameraData::getEndMagic()
{
    return slice(END_MAGIC_OFF, END_MAGIC_LEN);
}

CameraData::RGBType CameraData::getRGBType()
{
    int rgbExtern = this->getRGBExtern();
    if( rgbExtern == R ) {
        return R;
    } else if ( rgbExtern == G ) {
        return G;
    } else if( rgbExtern == B ) {
        return B;
    } else {
        qDebug() << "RGB 类型错误，类型数为 " << this->getRGBExtern();
        return UNKOWN;
    }

}

int CameraData::getPackageCntInLine()
{
    if(packageCntInLine == -1)
        packageCntInLine = sliceToInt(PARTIAL_CNT_OFF, PARTIAL_CNT_LEN);
    return packageCntInLine;
}

int CameraData::getPackageIdxInLine()
{
    if(packegeIdxInLine == -1) {
        packegeIdxInLine = sliceToInt(PARTIAL_IDX_OFF, PARTIAL_IDX_LEN);
    }
    return packegeIdxInLine;
}

int CameraData::getSampleDiffLine()
{
    int v = sliceToInt(SAMPLE_LINE_OFF, SAMPLE_LINE_LEN);
    return v;
}

int CameraData::getRGBExtern()
{
    if(rgbType == -1)
        rgbType = sliceToInt(RGB_EXTERN_OFF, RGB_EXTERN_LEN);
    return rgbType;
}

int CameraData::getLineIdx()
{
    if( lineIdx == -1 )
        lineIdx = sliceToInt(LINE_IDX_OFF, LINE_IDX_LEN);
//    qDebug() << "测试 LineIdx: " << test << v;
    return lineIdx;
}

int CameraData::getValidDataLen()
{
    int v = sliceToInt(VALID_DATALEN_OFF, VALID_DATALEN_LEN);
    return v;
}

int CameraData::getLineBytes()
{
    int v = sliceToInt(LINE_BYTES_OFF, LINE_BYTES_LEN);
    return v;
}

inline QByteArray CameraData::slice(int start, int len)
{
    return data.mid(start, len);
}

int CameraData::sliceToInt(int start, int len)
{
//    QByteArray ba = this->data.mid(start, len);
    int result = 0;
    if( len == 1 ) {
        result = uchar(this->data[start]);
    } else {
        result += uchar(this->data[start+3]);
        result += uchar(this->data[start+2]) << 8;
        result += uchar(this->data[start+1]) << 16;
        result += uchar(this->data[start+0]) << 24;
    }
    return result;
}

void CameraData::reset()
{
    DataObj::reset();
    packageCntInLine = -1;
    packegeIdxInLine = -1;
    lineIdx = -1;
    lineBytes = -1;
    rgbType = -1;
}


// ==================================================================


const int PreProcessData::PacketSize = 1280;
const int PreProcessData::PacketPerLine = 2;
PreProcessData::PreProcessData(QObject *parent, int line) :
    DataObj(parent),
    m_line(line)
{
    dataList.reserve(PacketPerLine * 3);
    dataPlaced = new bool[3*PacketPerLine];

    for(int i = 0; i < PacketPerLine * 3; i++) {
        dataPlaced[i] = false;
        QByteArray ba;
        ba.resize(PacketSize);
//        dataList[i] = QByteArray();
        dataList.append(ba);
    }
}

/**
 * @brief PreProcessData::put
 * @param cd
 * 填充数据前要确保行号一致
 */
void PreProcessData::put(CameraData *cd)
{
    int index = (int(cd->getRGBType()) - 1) * 2 + cd->getPackageIdxInLine();

    if( dataPlaced[index] == false ) {
//        dataList[index].data = *(cd->getData().data());
        memcpy(dataList[index].data(), cd->getData(), PacketSize);
        m_readyCount += 1;
        dataPlaced[index] = true;
    }
    else {
//        qDebug() << "接收到重复数据 Line: " << cd->getLineIdx() << index;
        m_duplicateCount += 1;
    }
}

bool PreProcessData::isReady()
{
    return m_readyCount == PacketPerLine * 3;
}

void PreProcessData::reset()
{
    DataObj::reset();
    m_readyCount = 0;
    for(int i = 0; i < PacketPerLine * 3; i++) {
        dataPlaced[i] = false;
    }
}

int PreProcessData::line() const
{
    return m_line;
}

void PreProcessData::setLine(int line)
{
    m_line = line;
}

const QByteArrayList &PreProcessData::getDataList() const
{
    return dataList;
}

QByteArray PreProcessData::repr()
{
    QByteArray r = QByteArray::number(m_line);
    for(int i = 0; i < PacketPerLine * 3; i++) {
        r.append(!dataList[i].isEmpty());
    }
    return r;
}
