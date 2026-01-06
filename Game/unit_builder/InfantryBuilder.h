#ifndef INFANTRYBUILDER_H
#define INFANTRYBUILDER_H

#include "unit_builder/UnitBuilder.h"
#include "units/Infantry.h"

class InfantryBuilder : public UnitBuilder
{
public:
    std::unique_ptr<Unit> build() override;
};

#endif // INFANTRYBUILDER_H
