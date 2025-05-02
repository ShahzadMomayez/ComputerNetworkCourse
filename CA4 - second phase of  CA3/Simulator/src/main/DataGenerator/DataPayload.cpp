#include "DataPayload.h"

DataPayload::DataPayload(const QByteArray &data, QObject *parent) :
    Payload {data, parent}
{}

PayloadPtr
DataPayload::clone()
{
    return PayloadPtr::create(data());
}
