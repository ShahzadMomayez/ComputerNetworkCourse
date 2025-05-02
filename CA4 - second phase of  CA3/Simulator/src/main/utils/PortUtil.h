#ifndef PORTUTIL_H
#define PORTUTIL_H

#include <QVector>
#include <tuple>
#include <QPair>
#include "../Port/Port.h"
#include "../Globals/Types.h"

class PortUtil
{

public:
    PortUtil();
    static  QPair<PortPtr, PortPtr> createLink();
};

#endif
