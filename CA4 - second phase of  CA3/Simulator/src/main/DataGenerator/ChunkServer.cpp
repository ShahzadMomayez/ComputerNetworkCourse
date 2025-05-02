#include "ChunkServer.h"

#include "DataPayload.h"

#include <QFile>

ChunkServer::ChunkServer(QObject *parent) :
    QObject {parent}, m_chunks(0), m_filePath(""), m_senderPCsCount(0), m_emptyChunk(0)
{}

ChunkServer *
ChunkServer::instance(QObject *parent)
{
    if(self == nullptr)
    {
        self = new ChunkServer(parent);
    }

    return self;
}

void
ChunkServer::release()
{
    delete self;
    self = nullptr;
}

QString
ChunkServer::filePath() const
{
    return m_filePath;
}

void
ChunkServer::setFilePath(const QString &newFilePath)
{
    m_filePath = newFilePath;
}

size_t
ChunkServer::packetSize() const
{
    return m_packetSize;
}

void
ChunkServer::setPacketSize(size_t newPacketSize)
{
    m_packetSize = newPacketSize;
}

QList<qsizetype>
ChunkServer::senderPCsIDs() const
{
    return m_senderPCsIDs;
}

void
ChunkServer::setSenderPCsIDs(const QList<qsizetype> &newSenderPCsIDs)
{
    m_senderPCsIDs = newSenderPCsIDs;
}

size_t
ChunkServer::senderPCsCount() const
{
    return m_senderPCsCount;
}

void
ChunkServer::setSenderPCsCount(size_t newSenderPCsCount)
{
    m_senderPCsCount = newSenderPCsCount;
}

void
ChunkServer::generateChunks()
{
    auto packets  = packetizeFile();
    int  partSize = packets.size() / m_senderPCsCount;

    for(size_t i = 0; i < m_senderPCsCount; ++i)
    {
        Chunk part = packets.mid(i * partSize, partSize);
        m_chunks.append(part);
    }

    int remaining = packets.size() % m_senderPCsCount;

    for(int i = 0; i < remaining; ++i)
    {
        m_chunks[i].append(packets[m_senderPCsCount * partSize + i]);
    }
}

ChunkServer::Chunk &
ChunkServer::loadChunk(qsizetype pcID)
{
    if(m_senderPCsIDs.contains(pcID))
    {
        return m_chunks[m_senderPCsIDs.indexOf(pcID)];
    }

    UT_WARNING "PC with ID" << pcID << "is not assigned a chunk yet.";
    return m_emptyChunk;
}

ChunkServer::Chunk
ChunkServer::packetizeFile()
{
    QFile file(m_filePath);

    if(!file.open(QIODevice::ReadOnly))
    {
        UT_ERROR "Unable to open file:" << m_filePath;
        qFatal("Unable to open file");
        return {};
    }

    QByteArray data = file.readAll();
    file.close();

    qDebug() << "\n\n"
             << UT_TerminalColorGreen << "Chunk Server: Read" << data.size()
             << "bytes from file:" << m_filePath << UT_TerminalColorReset << "\n";

    Chunk packets;

    for(qint64 i = 0; i < data.size(); i += m_packetSize)
    {
        QByteArray payload = data.mid(i, m_packetSize);
        auto       packet  = PacketPtr::create(UT::PacketType::Data, 0);
        DataPayloadPtr dataPayload = DataPayloadPtr::create(payload);
        packet->setPayload(dataPayload);
        packets.append(packet);
    }

    return packets;
}
