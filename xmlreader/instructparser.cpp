#include "instructparser.h"

#include <QFile>
#include <QDebug>

const double InstructParser::MinCompatableVersion = 2.0;

/*!
 * \class InstructParser
 * \ingroup protocol_layer
 * \author BriFuture
 * \date 2018.06.16
 *
 * \brief 从 XML 文件中读取可用的指令及相应的指令头
 */
InstructParser::InstructParser(QObject *parent) : QObject(parent)
{

}

InstructParser::~InstructParser() {
}

/*!
 * \brief 读取指定文件中的指令集
 *
 * \param filename 文件名
 * \param set 指令集名称
 *
 * 若指令集为空，则读取文件中的第一个指令集
 */
void InstructParser::parse(const QString &filename, const QString &set)
{
    // 打开并读取指令文件
    QFile file( filename );
    bool opened = file.open( QIODevice::ReadOnly );
    if( !opened ) {
        qDebug() << "[parse] file not open" ;
        return;
    }

    m_xmlReader = new QXmlStreamReader( &file );

    readInstructions(set);

    file.close();
    file.deleteLater();
    delete m_xmlReader;
}

void InstructParser::readInstructions(const QString &set)
{
    /* ![0]
    while( !m_xmlReader->atEnd() ) {
        m_xmlReader->readNext();
        qDebug() << "[readNext] "<< QString( "%0  %1").arg( m_xmlReader->lineNumber() ).arg( m_xmlReader->tokenString() ) << m_xmlReader->name().toString();
    }
    */
    /* ![1]
    while( !m_xmlReader->atEnd() ) {
        bool start = m_xmlReader->readNextStartElement();
        qDebug() << "[readNextStartElement] "<< start
                 << QString( "%0  %1").arg( m_xmlReader->lineNumber() ).arg( m_xmlReader->tokenString() )
                 << m_xmlReader->name().toString();
    }
    */
//    /* ![2]
    if( m_xmlReader->readNextStartElement() && m_xmlReader->name() == "Instructions" ) {
        m_attrib = m_xmlReader->attributes();
        m_version = m_attrib.value( "version" ).toDouble();
        Q_ASSERT( m_version >= MinCompatableVersion );
        bool firstRead = true;
        // 开始读取 XML 文件
        qDebug() << "[Instructions]" << m_xmlReader->name() << m_version << m_xmlReader->tokenString();
        while( m_xmlReader->readNextStartElement() ) {
            // 找到 Instruct 标签并读取，否则跳过标签
            if( m_xmlReader->name() == "Instruct" ) {
                readInstruct( set, firstRead );
                firstRead = false;
            }
            else {
                m_xmlReader->skipCurrentElement();
            }
        }
        qDebug() << "[Instructions]" << m_xmlReader->name() << m_attrib.value( "type" ) << m_xmlReader->tokenString();
    }
//    */
}

/*!
 * \brief InstructParser::readInstruct
 * \param set
 * \param stillRead
 *
 * 读取 Instruct 标签
 */
void InstructParser::readInstruct(const QString &set, const bool stillRead)
{
    m_attrib = m_xmlReader->attributes();
    QString instructSet = m_attrib.value( "set" ).toString();
    if( set.isEmpty() ) {
        if( !stillRead )  {
            m_xmlReader->skipCurrentElement();
            return;
        }
    }
    else if( set !=instructSet ) {
    // 若读取的指令集与需要的不匹配，则跳过该指令集
        m_xmlReader->skipCurrentElement();
        return;
    }
//    qDebug() << "[Inst]" << m_xmlReader->name() << m_attrib.value( "set" ) << m_xmlReader->tokenString();
    m_identifier  = m_attrib.value( "identifier" ).toLatin1();

    while( m_xmlReader->readNextStartElement() ) {
        if( m_xmlReader->name() == "row" ) {
            readRow();
        }
        else
            m_xmlReader->skipCurrentElement();
    }
//    m_xmlReader->readElementText();  // skip to end tag of instruct
}

/*!
 * \brief InstructParser::readRow
 *
 * 读取 row 标签
 */
void InstructParser::readRow()
{
    Instruct *ins = new Instruct( parent() );
    ins->setId( m_count );
    m_count ++;
    m_attrib = m_xmlReader->attributes();
    ins->setObjectName( m_attrib.value( "type" ).toString() );
    ins->setHeader( m_attrib.value( "Value" ).toString() );
    qDebug() << "[row]" << m_xmlReader->name() << m_attrib.value( "type" ) << m_xmlReader->tokenString();
    while( m_xmlReader->readNextStartElement() ) {
        if( m_xmlReader->name() == "reply" ) {
            readReply( ins );
        }
        else
            m_xmlReader->skipCurrentElement();
    }
//    m_xmlReader->readElementText();  // skip to end tag of row
    qDebug() << "[row]" << m_xmlReader->name() << m_attrib.value( "type" ) << m_xmlReader->tokenString();
}

/*!
 * \brief InstructParser::readReply
 * \param ins
 *
 * 读取 reply 标签
 */
void InstructParser::readReply(Instruct *ins)
{
    m_attrib = m_xmlReader->attributes();
    QString rType = m_attrib.value( "type" ).toString();
    ins->setReplyType( rType );
    bool enumable = (m_attrib.value( "enum" ) == "true");
    ins->setEnumable( enumable );

    qDebug() << "[reply]" << m_xmlReader->name() << m_attrib.value( "type" ) << m_xmlReader->tokenString();
    if( enumable ) {
        while( m_xmlReader->readNextStartElement() ) {
            if( m_xmlReader->name() == "accept" ) {
                m_attrib = m_xmlReader->attributes();
                ins->addAcceptable( m_attrib.value( "value" ).toString() );
                m_xmlReader->readElementText(); // skip to end tag of accept
                qDebug() << "[accept]" << m_xmlReader->name() << m_attrib.value( "value" ) << m_xmlReader->tokenString();
            }
            else
                m_xmlReader->skipCurrentElement();
        }
    }
//    m_xmlReader->readElementText();  // skip to end tag of reply
    qDebug() << "[reply]" << m_xmlReader->name() << m_attrib.value( "type" ) << m_xmlReader->tokenString();
}


QByteArray InstructParser::identifier() const
{
    if( m_identifier.isEmpty() )
        return QByteArray( "#" );  // set Defalut Identifier

    return m_identifier;
}
