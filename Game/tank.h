#ifndef TANK_H
#define TANK_H

#include "Unit.h"

class Tank : public Unit
{
public:
    explicit Tank(Player* owner);
};

#endif // TANK_H
