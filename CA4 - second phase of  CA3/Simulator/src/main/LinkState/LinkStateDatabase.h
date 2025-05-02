#ifndef LINKSTATEDATABASE_H
#define LINKSTATEDATABASE_H
#include <QObject>
#include <QMap>
#include <QVector>
#include <QMutex>
#include "LinkState.h"

class LinkStateDatabase : public QObject
{
    Q_OBJECT

public:
    explicit LinkStateDatabase(QObject *parent = nullptr);
    void add(int node, int cost, int neighborId, IPPtr neighborIP);
    QList<int> nodes();
    QVector<LinkStatePtr> edges(int node);
    void lock();
    void unlock();
    IPPtr getIP(int id);

private:
    QMutex mutex;
    QMap<int, QVector<LinkStatePtr>> database;
};

typedef QSharedPointer<LinkStateDatabase> LinkStateDatabasePtr;

#endif
