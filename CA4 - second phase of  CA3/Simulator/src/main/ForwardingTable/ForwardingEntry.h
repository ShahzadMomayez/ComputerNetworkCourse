#ifndef FORWARDINGENTRY_H
#define FORWARDINGENTRY_H

#include <QObject>
#include "../IP/IP.h"
#include "../Globals/Globals.h"


class ForwardingEntry;
typedef QSharedPointer<ForwardingEntry> ForwardingEntryPtr;

class ForwardingEntry : public QObject
{
    Q_OBJECT

public:
    explicit ForwardingEntry(IPPtr destination, IPPtr mask, IPPtr gateway, int metric, UT::RoutingProtocolType protocol=UT::RoutingProtocolType::RIP, QObject *parent = nullptr);
    bool equals(IPPtr destination);
    IPPtr getGateway();
    int getMetric();
    IPPtr getDestination();
    QString toString();
    bool isBetter(ForwardingEntryPtr newEntry);
    ForwardingEntryPtr clone(IPPtr gateway, int metric);
    ForwardingEntryPtr clone();

private:
    IPPtr destination;
    IPPtr mask;
    IPPtr gateway;
    int metric;
    UT::RoutingProtocolType protocol;
};

#endif
