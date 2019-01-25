#include "cameradatapool.h"
#include <QDebug>


template<class T>
DataObjPool<T>::DataObjPool(int size)
{
    T *pt;
    pool.reserve(size);
    for(int i = 0; i < size; i++) {
        pt = new T();
        pt->setValid(true);
        pt->setId(i);
        pool.append(pt);
    }
    numAvailable = size;
}

template <class T> template <class T2>
DataObjPool<T>::operator DataObjPool<T2>() {
    Stack<T2> StackT2;
    for (int i = 0; i < m_size; i++) {
        StackT2.push((T2)m_pT[m_size - 1]);
    }
    return StackT2;
}


RawDataObjPool::RawDataObjPool(int size) : DataObjPool(size)
{
}

LineDataPool::LineDataPool(int size) : DataObjPool(size)
{

}
