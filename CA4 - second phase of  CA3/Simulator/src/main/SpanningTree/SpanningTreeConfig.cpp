#include "SpanningTreeConfig.h"

SpanningTreeConfig::SpanningTreeConfig(int rootId, int distToRoot, int senderId, int parentId,
                                       QObject *parent) :
    Payload {"", parent}
{
    this->senderId = senderId;
    this->rootId = rootId;
    this->distToRoot = distToRoot;
    this->parentId = parentId;
}

bool SpanningTreeConfig::isBetter(SpanningTreeConfigPtr other) {
    if (rootId < other->rootId) {
        return true;
    }
    if ((rootId == other->rootId) && (distToRoot + 1 < other->distToRoot)) {
        return true;
    }
    if ((rootId == other->rootId) && (distToRoot + 1 == other->distToRoot) && (senderId < other->senderId)) {
        return true;
    }
    return false;
}   

bool SpanningTreeConfig::indicateLoop(SpanningTreeConfigPtr newConfig, int id) {
    bool duplicatedIncommingConfig = (senderId == newConfig->senderId) &&
                                     (rootId == newConfig->rootId) &&
                                     (distToRoot == newConfig->distToRoot + 1);

    return rootId == newConfig->rootId && newConfig->parentId != id && !duplicatedIncommingConfig;
}

SpanningTreeConfigPtr SpanningTreeConfig::clone(int newSenderId) {
    return SpanningTreeConfigPtr(new SpanningTreeConfig(rootId, distToRoot, newSenderId, senderId));
}

void SpanningTreeConfig::update(SpanningTreeConfigPtr newConfig) {
    rootId = newConfig->rootId;
    distToRoot = newConfig->distToRoot + 1;
    senderId = newConfig->senderId;
}

int SpanningTreeConfig::getSenderId() {
    return senderId;
}

int SpanningTreeConfig::getRootId() {
    return rootId;
}
