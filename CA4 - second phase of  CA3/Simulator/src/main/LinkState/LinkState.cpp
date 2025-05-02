#include "LinkState.h"

LinkState::LinkState(int cost, int neighborId, IPPtr neighborIP, QObject *parent) {
    this->cost = cost;
    this->neighborId = neighborId;
    this->neighborIP = neighborIP;
}

int LinkState::getCost() {
    return cost;
}

int LinkState::getNeighborId() {
    return neighborId;
}

IPPtr LinkState::getNeighborIP() {
    return neighborIP;
}
