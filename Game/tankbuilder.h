#ifndef TANKBUILDER_H
#define TANKBUILDER_H

#include "UnitBuilder.h"
#include "Tank.h"

class TankBuilder : public UnitBuilder
{
public:
    std::unique_ptr<Unit> build() override;
};
#endif // TANKBUILDER_H
