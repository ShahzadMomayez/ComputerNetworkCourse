#include "TCPHeader.h"

TCPHeader::TCPHeader(QObject *parent) :
    QObject {parent}
{}

uint16_t
TCPHeader::sourcePort() const
{
    return m_sourcePort;
}

void
TCPHeader::setSourcePort(uint16_t newSourcePort)
{
    m_sourcePort = newSourcePort;
}

uint16_t
TCPHeader::destPort() const
{
    return m_destPort;
}

void
TCPHeader::setDestPort(uint16_t newDestPort)
{
    m_destPort = newDestPort;
}

uint32_t
TCPHeader::sequenceNumber() const
{
    return m_sequenceNumber;
}

void
TCPHeader::setSequenceNumber(uint32_t newSequenceNumber)
{
    m_sequenceNumber = newSequenceNumber;
}

uint32_t
TCPHeader::acknowledgmentNumber() const
{
    return m_acknowledgmentNumber;
}

void
TCPHeader::setAcknowledgmentNumber(uint32_t newAcknowledgmentNumber)
{
    m_acknowledgmentNumber = newAcknowledgmentNumber;
}

uint8_t
TCPHeader::dataOffset() const
{
    return m_dataOffset;
}

void
TCPHeader::setDataOffset(uint8_t newDataOffset)
{
    m_dataOffset = newDataOffset;
}

UT::PacketControlType
TCPHeader::flags() const
{
    return m_flags;
}

void
TCPHeader::setFlags(UT::PacketControlType newFlags)
{
    m_flags = newFlags;
}

uint16_t
TCPHeader::windowSize() const
{
    return m_windowSize;
}

void
TCPHeader::setWindowSize(uint16_t newWindowSize)
{
    m_windowSize = newWindowSize;
}

uint16_t
TCPHeader::checksum() const
{
    return m_checksum;
}

void
TCPHeader::setChecksum(uint16_t newChecksum)
{
    m_checksum = newChecksum;
}

uint16_t
TCPHeader::urgentPointer() const
{
    return m_urgentPointer;
}

void
TCPHeader::setUrgentPointer(uint16_t newUrgentPointer)
{
    m_urgentPointer = newUrgentPointer;
}
