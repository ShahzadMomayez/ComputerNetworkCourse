#ifndef FORWARDINGTABLE_H
#define FORWARDINGTABLE_H

#include "ForwardingEntry.h"
#include "../IP/IP.h"
#include "../IP/IPv4.h"

#include <QObject>
#include <QVector>


class ForwardingTable;
typedef QSharedPointer<ForwardingTable> ForwardingTablePtr; 

class ForwardingTable : public QObject
{
    Q_OBJECT

public:
    explicit ForwardingTable(QObject *parent = nullptr);
    ~ForwardingTable();
    IPPtr getGatway(IPPtr destination);
    void addEntry(ForwardingEntryPtr entry);
    void replaceEntry(ForwardingEntryPtr oldEntry, ForwardingEntryPtr newEntry);
    void addEntries(QVector<ForwardingEntryPtr> entries);
    QVector<ForwardingEntryPtr> getEntries();
    ForwardingEntryPtr findEntry(IPPtr destination);
    QString show();
    ForwardingTablePtr clone();
    void clear();

private:
    QVector<ForwardingEntryPtr> entries;
};

#endif
