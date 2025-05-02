#ifndef SPANNINGTREECONFIG_H
#define SPANNINGTREECONFIG_H

#include <QObject>
#include <QSharedPointer>
#include "../Payload/Payload.h"

class SpanningTreeConfig;
typedef QSharedPointer<SpanningTreeConfig> SpanningTreeConfigPtr;

class SpanningTreeConfig : public Payload
{
    Q_OBJECT

public:
    explicit SpanningTreeConfig(int rootId, int distToRoot, int senderId, int parentId, QObject *parent = nullptr);
    bool isBetter(SpanningTreeConfigPtr other);
    bool indicateLoop(SpanningTreeConfigPtr newConfig, int id);
    void update(SpanningTreeConfigPtr newConfig);
    SpanningTreeConfigPtr clone(int newSenderId);
    int getSenderId();
    int getRootId();

Q_SIGNALS:

private:
    int distToRoot;
    int rootId;
    int senderId;
    int parentId;

};


#endif
