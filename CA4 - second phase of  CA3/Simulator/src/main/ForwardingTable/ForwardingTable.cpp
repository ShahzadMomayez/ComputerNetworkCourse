#include "ForwardingTable.h"
#include <QDebug>

ForwardingTable::ForwardingTable(QObject *parent) {}

ForwardingTable::~ForwardingTable() {
        // qDebug() << "table destroyed -----------------------------------";
}

IPPtr ForwardingTable::getGatway(IPPtr destination) {
    QVector<ForwardingEntryPtr> gateways;
    for (ForwardingEntryPtr& entry : entries) {
        if (entry->equals(destination)) {
            gateways.append(entry);
        }
    }
    auto minEntry = std::min_element(gateways.begin(), gateways.end(), [](ForwardingEntryPtr &a, ForwardingEntryPtr &b) {
        return a->getMetric() < b->getMetric();
    });
    
    if (minEntry == gateways.end()) {
        return nullptr;
    }
    return (*minEntry)->getGateway();
}

void ForwardingTable::addEntry(ForwardingEntryPtr entry) {
    entries.append(entry);
}

void ForwardingTable::addEntries(QVector<ForwardingEntryPtr> entries) {
    this->entries.append(entries);
}

QString ForwardingTable::show() {
    QString result;
    result += " Destination      Subnet Mask       Gateway     Metric Protocol";
    for (const auto &entry : entries) {
        result += "\n" + entry->toString();
    }
    return result;
}

QVector<ForwardingEntryPtr> ForwardingTable::getEntries() {
    return entries;
}

void ForwardingTable::clear() {
    entries.clear();
}

ForwardingEntryPtr ForwardingTable::findEntry(IPPtr destination) {
    for (auto entry : entries) {
        if (entry->equals(destination)) {
            return entry;
        }
    }
    return nullptr;
}

void ForwardingTable::replaceEntry(ForwardingEntryPtr oldEntry, ForwardingEntryPtr newEntry) {
    int index = entries.indexOf(oldEntry);
    if (index == -1) {
        entries.append(newEntry);
    }
    else {
        entries[index] = newEntry;
    }
    
}

ForwardingTablePtr ForwardingTable::clone() {
    auto forwardingtable = ForwardingTablePtr(new ForwardingTable());
    for (auto entry : entries) {
        forwardingtable->addEntry(entry->clone());
    }
    return forwardingtable;
}
