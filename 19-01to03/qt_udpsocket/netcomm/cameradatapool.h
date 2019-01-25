#ifndef CAMERADATAPOOL_H
#define CAMERADATAPOOL_H

#include <QObject>
#include <QMap>
#include <QDebug>

#include "cameradata.h"


template<class T>
class DataObjPool
{
public:
    virtual ~DataObjPool() {
        qDeleteAll(pool);
        numAvailable = 0;
    }

    T *getAvailable() {
        if( numAvailable == 0 ) {
            return 0;
        }
        for(int i = 0; i < pool.size(); i++) {
            T *item = pool[i];
            if(item->isValid()) {
                item->setValid(false);
                numAvailable -= 1;
                return item;
            }
        }
        return 0;
    }

    T *get(int id) {
        return pool[id];
    }

    inline bool release(T *dobj) {
        dobj->reset();
        numAvailable += 1;
        return true;
    }

    int releaseTimeout(int now, int timeout = 100) {
        int releaseCount = 0;
        for(int i = 0; i < pool.size(); i++) {
            T *item = pool[i];
            if(now > item->getGenerateMs() + timeout) {
                item->reset();
                numAvailable += 1;
                releaseCount += 1;
            }
        }
        return releaseCount;
    }

    void releaseAll() {
        for(int i = 0; i < pool.size(); i++) {
            T *item = pool[i];
            if(item->isValid()) {
                continue;
            }
            item->reset();
            numAvailable += 1;
        }
    }

    int getNumAvailable() const {
        return numAvailable;
    }
    template<class T2>  operator DataObjPool<T2>();

protected:
    DataObjPool(int size = 100);
private:
    QList<T *> pool;
    int numAvailable = 0;
};

class RawDataObjPool: public DataObjPool<CameraData>
{
public:
    RawDataObjPool(int size = 100);
};

class LineDataPool : public DataObjPool<PreProcessData>
{
public:
    LineDataPool(int size = 100);
};



#endif // CAMERADATAPOOL_H
