#ifndef INFANTRY_H
#define INFANTRY_H

#include "unit_builder/Unit.h"

class Infantry : public Unit
{
public:
    explicit Infantry(Player* owner);
};

#endif // INFANTRY_H
