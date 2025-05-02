#include "LinkStatePayload.h"

LinkStatePayload::LinkStatePayload(int senderId) :
    Payload {""}, senderId(senderId)
{}

void LinkStatePayload::addLink(int cost, int neighborId, IPPtr neighborIP) {
    links.append(LinkStatePtr(new LinkState(cost, neighborId, neighborIP)));
}

QVector<LinkStatePtr> LinkStatePayload::getLinks() const {
    return links;
}

int LinkStatePayload::getSenderId() const {
    return senderId;
}

PayloadPtr LinkStatePayload::clone() {
    auto other = LinkStatePayloadPtr(new LinkStatePayload(senderId));
    other->links = links;
    return other;
}
