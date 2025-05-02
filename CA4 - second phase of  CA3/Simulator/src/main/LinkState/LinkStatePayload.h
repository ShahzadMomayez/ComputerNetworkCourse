#ifndef LINKSTATEPAYLOAD_H
#define LINKSTATEPAYLOAD_H

#include "../Payload/Payload.h"
#include "LinkState.h"
#include <QVector>

class LinkStatePayload : public Payload {
    Q_OBJECT

public:
    explicit LinkStatePayload(int senderId);
    void addLink(int cost, int neighborId, IPPtr neighborIP);
    QVector<LinkStatePtr> getLinks() const;
    int getSenderId() const;
    PayloadPtr clone();

private:
    int senderId;
    QVector<LinkStatePtr> links;
};

typedef QSharedPointer<LinkStatePayload> LinkStatePayloadPtr;

#endif
