#ifndef INSTRUCT_H
#define INSTRUCT_H

#include <QObject>
#include <QVector>

class Instruct: public QObject
{
    Q_OBJECT
public:
    enum AcceptReplyType {
        String,
        Double
    };

    explicit Instruct(QObject *parent = 0);
    ~Instruct();

    QString header() const;
    void setHeader(const QString &header);

    bool isEnumable() const;
    void setEnumable(bool enumable);

    void addAcceptable(const QString &accpet);
    bool isAcceptable(const QString &test);

    AcceptReplyType replyType() const;
    void setReplyType(const AcceptReplyType &replyType);
    void setReplyType(const QString &replyType);

    int id() const;
    void setId(int id);


signals:

public slots:

private:
    int m_id;
    QString m_header;
    bool m_enumable = false;
    AcceptReplyType m_replyType = String;
    QVector<QString> *m_acceptable = Q_NULLPTR;
};

#endif // INSTRUCT_H
