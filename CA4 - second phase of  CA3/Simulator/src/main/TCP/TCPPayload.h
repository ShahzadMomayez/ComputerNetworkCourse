#ifndef TCPPAYLOAD_H
#define TCPPAYLOAD_H

#include "../Payload/Payload.h"

#include <QObject>

class TCPPayload : public Payload
{
    Q_OBJECT

public:
    explicit TCPPayload(const QByteArray &data, QObject *parent = nullptr);
};

#endif    // TCPPAYLOAD_H
