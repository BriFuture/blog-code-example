#ifndef INSTRUCTPARSER_H
#define INSTRUCTPARSER_H

#include <QObject>
#include <QXmlStreamReader>
#include <QXmlStreamAttributes>
#include <QVector>

#include "instruct.h"

class InstructParser : public QObject
{
    Q_OBJECT
public:
    static const double MinCompatableVersion;
    explicit InstructParser(QObject *parent = 0);
    ~InstructParser();
    void parse(const QString &filename, const QString &set = QString());

    QByteArray identifier() const;

signals:

private:
    void readInstructions(const QString &set);
    void readInstruct(const QString &set, const bool stillRead = false);
    void readRow();
    void readReply(Instruct *ins);

private:
    // 用 xml 流式读取
    QXmlStreamReader *m_xmlReader;
    QXmlStreamAttributes m_attrib;
    double m_version = 0.0;
    QByteArray m_identifier;
    QByteArray m_instructSet;

    int m_count = 0;
};

#endif // INSTRUCTPARSER_H
