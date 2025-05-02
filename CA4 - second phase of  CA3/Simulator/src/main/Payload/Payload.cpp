#include "Payload.h"

Payload::Payload(const QByteArray &data, QObject *parent) :
    QObject {parent}, m_data(data)
{}

PayloadPtr Payload::clone() {
    return nullptr;
}

QByteArray
Payload::data() const
{
    return m_data;
}

void
Payload::setData(const QByteArray &newData)
{
    m_data = newData;
}
