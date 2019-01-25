#include <QObject>
/**
 * @brief The ProcessedData class
 * 将原始数据包拼接后的数据类，不执行拼接操作
 * 该类中的数据为一行数据。包含 RGB 3 个通道的值，每个通道 2096 个字节
 */
class ProcessedLineData: public DataObj
{
    Q_OBJECT

public:
    explicit ProcessedLineData(QObject *parent = 0, int line = -1);
    ~ProcessedLineData();
    // 设置和返回单行单通道的值
    void setR(const QByteArray &r);
    QByteArray &r();
    void setG(const QByteArray &g);
    QByteArray &g();
    void setB(const QByteArray &b);
    QByteArray &b();

    // 返回该行 3 通道的值，三个通道依次间隔插入形成一行值。共 2096 * 3 个字节
    // 排列方式为 rgbrgbrgb，r、g、b 分别为 0~255 之间的值
    QByteArray rgb();
    // 判断三个通道的数据是否都就绪了
    bool isReady();
    bool isValid();
    void setLine(int line);
    int line();
    int pixelPerLine();

private:
    QByteArray rline;
    QByteArray gline;
    QByteArray bline;
    int m_line = -1;
    int rcount = 0;
    int gcount = 0;
    int bcount = 0;
};


/**
 * @brief ProcessedData::ProcessedData
 * @param rawData
 * @param parent
 */
ProcessedLineData::ProcessedLineData(QObject *parent, int line) : DataObj(parent)
{
    m_line = line;
}

ProcessedLineData::~ProcessedLineData()
{
    rline.clear();
    bline.clear();
    gline.clear();
}

void ProcessedLineData::setR(const QByteArray &r)
{
    rcount = r.size();
    this->rline = r;
}

QByteArray &ProcessedLineData::r()
{
    return this->rline;
}

void ProcessedLineData::setG(const QByteArray &g)
{
    gcount = g.size();
    this->gline = g;
}

QByteArray &ProcessedLineData::g()
{
    return this->gline;
}

void ProcessedLineData::setB(const QByteArray &b)
{
    bcount = b.size();
    this->bline = b;
}

QByteArray &ProcessedLineData::b()
{
    return this->bline;
}

QByteArray ProcessedLineData::rgb()
{
    QByteArray ba;
    int len = pixelPerLine();
    ba.reserve(len * 3);
    for(int i = 0; i < len; i++) {
        ba[i*3+0] = this->rline[i];
        ba[i*3+1] = this->gline[i];
        ba[i*3+2] = this->bline[i];
    }
    return ba;
}

bool ProcessedLineData::isReady()
{
    return (rcount > 0) && (gcount > 0) && (bcount > 0);
}

bool ProcessedLineData::isValid()
{
    return (rcount == gcount) && (rcount == bcount);
}

int ProcessedLineData::line()
{
    return this->m_line;
}

int ProcessedLineData::pixelPerLine()
{
    return this->rline.length();
}

void ProcessedLineData::setLine(int line)
{
    m_line = line;
}

// ===================

class LineDataPool: public QObject
{
    Q_OBJECT

public:

    explicit LineDataPool(QObject *parent = 0, int size = 100);
    ProcessedLineData *getAvailable();
    void release(ProcessedLineData *pld);

    int getAvailableCount() const;

private:
//    QMap<int, ProcessedLineData *> pool;
    QList<ProcessedLineData *> pool;
    int availableCount = 0;
};


// ===========

LineDataPool::LineDataPool(QObject *parent, int size) : QObject(parent)
{
    ProcessedLineData *pld;
    for(int i = 0; i < size; i++) {
        pld = new ProcessedLineData(this);
        pld->setId(i);
        pld->setValid(true);
//        pool.insert(i, pld);
        pool.append(pld);
    }
    availableCount = size;
}

ProcessedLineData *LineDataPool::getAvailable()
{
    if( availableCount == 0 ) {
        return 0;
    }
    auto it = pool.begin() + (pool.size() - availableCount);
    (*it)->setValid(false);
    availableCount -= 1;
    return *it;
}

void LineDataPool::release(ProcessedLineData *pld)
{
    pld->setValid(true);
    availableCount += 1;
}

int LineDataPool::getAvailableCount() const
{
    return availableCount;
}
