#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <QObject>
#include <QSharedPointer>

class Payload;
typedef QSharedPointer<Payload> PayloadPtr;

class Payload : public QObject
{
    Q_OBJECT

public:
    explicit Payload(const QByteArray &data, QObject *parent = nullptr);

    virtual PayloadPtr clone();

    QByteArray         data() const;
    void               setData(const QByteArray &newData);

private:
    QByteArray m_data;
};

typedef QSharedPointer<Payload> PayloadPtr;
#endif    
