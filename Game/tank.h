#ifndef TANK_H
#define TANK_H
#include "unit.h"
class Tank: public Unit
{
public:
    Tank();

    QString getType() const override { return "Tank"; }
};

#endif // TANK_H
