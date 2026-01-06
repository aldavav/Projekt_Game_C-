#ifndef TANKBUILDER_H
#define TANKBUILDER_H

#include "unit_builder/UnitBuilder.h"
#include "units/Tank.h"

class TankBuilder : public UnitBuilder
{
public:
    std::unique_ptr<Unit> build() override;
};
#endif // TANKBUILDER_H
