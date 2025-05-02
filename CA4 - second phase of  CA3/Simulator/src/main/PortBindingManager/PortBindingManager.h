#ifndef PORTBINDINGMANAGER_H
#define PORTBINDINGMANAGER_H

#include "../Port/Port.h"

#include <QList>
#include <QMap>
#include <QObject>

class PortBindingManager : public QObject
{
    Q_OBJECT

public:
    explicit PortBindingManager(QObject *parent = nullptr);
    ~PortBindingManager() override = default;

    void bind(const PortPtr &port1, const PortPtr &port2);
    bool unbind(const PortPtr &port1, const PortPtr &port2);

Q_SIGNALS:
    // for testing purposes
    void bindingChanged(const QString &router1, uint8_t port1, const QString &router2,
                        uint8_t port2, bool bind);

private:
    QMap<PortPtr, QList<PortPtr>> bindings;
};

#endif    // PORTBINDINGMANAGER_H
