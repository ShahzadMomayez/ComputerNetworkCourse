#ifndef TCPHEADER_H
#define TCPHEADER_H

#include "../Globals/Globals.h"

#include <cstdint>

#include <QObject>

class TCPHeader : public QObject
{
    Q_OBJECT

public:
    explicit TCPHeader(QObject *parent = nullptr);

    uint16_t sourcePort() const;
    void     setSourcePort(uint16_t newSourcePort);

    uint16_t destPort() const;
    void     setDestPort(uint16_t newDestPort);

    uint32_t sequenceNumber() const;
    void     setSequenceNumber(uint32_t newSequenceNumber);

    uint32_t acknowledgmentNumber() const;
    void     setAcknowledgmentNumber(uint32_t newAcknowledgmentNumber);

    uint8_t  dataOffset() const;
    void     setDataOffset(uint8_t newDataOffset);

    UT::PacketControlType flags() const;
    void                  setFlags(UT::PacketControlType newFlags);

    uint16_t windowSize() const;
    void     setWindowSize(uint16_t newWindowSize);

    uint16_t checksum() const;
    void     setChecksum(uint16_t newChecksum);

    uint16_t urgentPointer() const;
    void     setUrgentPointer(uint16_t newUrgentPointer);

Q_SIGNALS:

private:
    uint16_t              m_sourcePort;
    uint16_t              m_destPort;
    uint32_t              m_sequenceNumber;
    uint32_t              m_acknowledgmentNumber;
    uint8_t               m_dataOffset;
    UT::PacketControlType m_flags;    // URG, ACK, PSH, RST, SYN, FIN
    uint16_t              m_windowSize;
    uint16_t              m_checksum;
    uint16_t              m_urgentPointer;
};

typedef QSharedPointer<TCPHeader> TCPHeaderPtr;
#endif
