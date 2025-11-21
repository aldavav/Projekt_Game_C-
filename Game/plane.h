#ifndef PLANE_H
#define PLANE_H
#include "unit.h"
class Plane: public Unit
{
public:
    Plane();
    QString getType() const override { return "BattlePlane"; }
};

#endif // PLANE_H
