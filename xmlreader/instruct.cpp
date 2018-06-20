#include "instruct.h"

Instruct::Instruct(QObject *parent) : QObject(parent)
{
    m_id = -1;
}

Instruct::~Instruct()
{
    delete m_acceptable;
}

QString Instruct::header() const
{
    return m_header;
}

void Instruct::setHeader(const QString &header)
{
    m_header = header;
}

bool Instruct::isEnumable() const
{
    return m_enumable;
}

void Instruct::setEnumable(bool enumable)
{
    m_enumable = enumable;
    if( enumable ) {
        m_acceptable = new QVector<QString>;
    }
}

void Instruct::addAcceptable(const QString &accpet)
{
    if( m_enumable ) {
        m_acceptable->append( accpet );
    }
}

bool Instruct::isAcceptable(const QString &test)
{
    bool acceptable = true;
    if( m_replyType == Double ) {
        bool ok = false;
        double testD = test.toDouble( &ok );
        acceptable = ok;
        // 如果数字的值也只有几个允许的，那么设置其值
        if( m_enumable ) {
            bool near = false;
            for(int i = 0; i < m_acceptable->length(); i++ ) {
                double enumNum = m_acceptable->at( i ).toDouble();
                if( abs( enumNum - testD ) < 0.001 ) {
                    near = true;
                }
            }
            acceptable &= near;  // must be double and in Acceptable List
        }
    }
    else if( m_replyType == String ) {
        if( m_enumable ) {
            acceptable = m_acceptable->contains( test );
        }
    }

    // return true default
    return acceptable;
}

Instruct::AcceptReplyType Instruct::replyType() const
{
    return m_replyType;
}

void Instruct::setReplyType(const AcceptReplyType &replyType)
{
    m_replyType = replyType;
}

void Instruct::setReplyType(const QString &replyType)
{
    if( replyType == "String" ) {
        m_replyType = String;
    }
    else if( replyType == "Double" ) {
        m_replyType = Double;
    }
}

int Instruct::id() const
{
    return m_id;
}

void Instruct::setId(int id)
{
    m_id = id;
}
