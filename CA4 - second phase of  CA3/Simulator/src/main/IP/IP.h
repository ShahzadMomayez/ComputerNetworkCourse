#ifndef IP_H
#define IP_H

#include "../Globals/Globals.h"

#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QTextStream>

template <UT::IPVersion version>
class IP;

class AbstractIP;
typedef QSharedPointer<AbstractIP>  IPPtr;

class AbstractIP : public QObject
{
    Q_OBJECT

public:
    explicit AbstractIP(QObject *parent = nullptr);
    virtual IPPtr bitwiseAnd(IPPtr other) = 0;
    virtual bool equals(IPPtr other) = 0;
    virtual QString toString() = 0;
    virtual IPPtr clone() = 0;

Q_SIGNALS:

protected:

};

#endif
