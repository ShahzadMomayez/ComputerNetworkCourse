#ifndef DATAPAYLOAD_H
#define DATAPAYLOAD_H

#include "../Payload/Payload.h"

#include <QObject>

class DataPayload : public Payload
{
    Q_OBJECT

public:
    explicit DataPayload(const QByteArray &data, QObject *parent = nullptr);

    virtual PayloadPtr clone() override;

Q_SIGNALS:
};

typedef QSharedPointer<DataPayload> DataPayloadPtr;

#endif    // DATAPAYLOAD_H
