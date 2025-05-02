#ifndef LINKSTATE_H
#define LINKSTATE_H

#include <QObject>
#include <QSharedPointer>

#include "../IP/IPv4.h"

class LinkState : public QObject
{
    Q_OBJECT

public:
    explicit LinkState(int cost, int neighborId, IPPtr neighborIP, QObject *parent = nullptr);

    int getCost();
    int getNeighborId();
    IPPtr getNeighborIP();

private:
    int cost;
    int neighborId;
    IPPtr neighborIP;
};


typedef QSharedPointer<LinkState> LinkStatePtr;
#endif
