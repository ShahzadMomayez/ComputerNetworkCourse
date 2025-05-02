#include "TCPPayload.h"

TCPPayload::TCPPayload(const QByteArray &data, QObject *parent) :
    Payload {data, parent}
{}
