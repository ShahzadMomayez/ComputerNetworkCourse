#include "LinkStateDatabase.h"
#include <QDebug>

LinkStateDatabase::LinkStateDatabase(QObject *parent) :
    QObject {parent}
{}

void LinkStateDatabase::add(int node, int cost, int neighborId, IPPtr neighborIP) {
    mutex.lock();

    if (!database.contains(node)) {
        database[node] = QVector<LinkStatePtr>();
    }

    database[node].append(LinkStatePtr(new LinkState(cost, neighborId, neighborIP)));

    mutex.unlock();
}

QList<int> LinkStateDatabase::nodes() {
    return database.keys();
}

QVector<LinkStatePtr> LinkStateDatabase::edges(int node) {

    return database[node];
}

void LinkStateDatabase::lock() {
    mutex.lock();
}

void LinkStateDatabase::unlock() {
    mutex.unlock();
}

IPPtr LinkStateDatabase::getIP(int id) {
    for(auto links : database.values()) {
        for (auto link : links) {
            if (link->getNeighborId() == id) {
                return link->getNeighborIP();
            }
        }
    }
    qDebug() << "NOt found ---------------------------------------------------------------";
    return nullptr;
}
