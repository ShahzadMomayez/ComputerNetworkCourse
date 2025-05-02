#ifndef CHUNKSERVER_H
#define CHUNKSERVER_H

#include "../Packet/Packet.h"

#include <QObject>

class ChunkServer : public QObject
{
    Q_OBJECT

    explicit ChunkServer(QObject* parent = nullptr);

public:
    typedef QList<PacketPtr> Chunk;
    typedef QList<Chunk>     ChunkList;

public:
    static ChunkServer* instance(QObject* parent = nullptr);
    static void         release();

    void                generateChunks();
    Chunk&              loadChunk(qsizetype chunkIndex);

    QString             filePath() const;
    void                setFilePath(const QString& newFilePath);

    size_t              packetSize() const;
    void                setPacketSize(size_t newPacketSize);

    QList<qsizetype>    senderPCsIDs() const;
    void                setSenderPCsIDs(const QList<qsizetype>& newSenderPCsIDs);

    size_t              senderPCsCount() const;
    void                setSenderPCsCount(size_t newSenderPCsCount);

private:
    Chunk packetizeFile();

private:
    inline static ChunkServer* self = nullptr;

    ChunkList                  m_chunks;
    QString                    m_filePath;
    size_t                     m_packetSize;
    size_t                     m_senderPCsCount;
    QList<qsizetype>           m_senderPCsIDs;
    Chunk                      m_emptyChunk;
};

#endif    // CHUNKSERVER_H
