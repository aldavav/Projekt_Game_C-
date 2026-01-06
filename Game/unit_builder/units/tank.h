#ifndef TANK_H
#define TANK_H

#include "unit_builder/Unit.h"

class Tank : public Unit
{
public:
    explicit Tank(Player* owner);
};

#endif // TANK_H
