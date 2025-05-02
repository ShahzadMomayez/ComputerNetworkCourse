#include "ForwardingEntry.h"
#include "../utils/RoutingProtocolTypeUtil.h"
#include "qdebug.h"
#include <QVector>


ForwardingEntry::ForwardingEntry(IPPtr destination, IPPtr mask, IPPtr gateway, int metric, UT::RoutingProtocolType protocol, QObject *parent) {
    this->destination = destination;
    this->mask = mask;
    this->gateway = gateway;
    this->metric = metric;
    this->protocol = protocol;
}

bool ForwardingEntry::equals(IPPtr destination) {
    return this->destination->bitwiseAnd(mask)->equals(destination->bitwiseAnd(mask));
}

IPPtr ForwardingEntry::getGateway() {
    return gateway;
}

int ForwardingEntry::getMetric() {
    return metric;
}

QString ForwardingEntry::toString() {
    QString result = destination->toString().leftJustified(14, ' ');
    result += "  ";
    result += mask->toString();
    result += "  "; 
    result += gateway->toString().leftJustified(14, ' ');
    result += "    ";
    result += QString::number(metric);
    result += "     ";
    result += RoutingProtocolTypeUtil::toString(protocol);
    return result;
}

bool ForwardingEntry::isBetter(ForwardingEntryPtr oldEntry) {
    if (oldEntry == nullptr) {
        return true;
    }
    return metric + 1< oldEntry->metric;
}

IPPtr ForwardingEntry::getDestination() {
    return destination;
}

ForwardingEntryPtr ForwardingEntry::clone(IPPtr gateway, int metric) {
    return ForwardingEntryPtr(new ForwardingEntry(destination, mask, gateway, metric, protocol));
}

ForwardingEntryPtr ForwardingEntry::clone() {
    return ForwardingEntryPtr(new ForwardingEntry(destination, mask, gateway, metric, protocol));
}
