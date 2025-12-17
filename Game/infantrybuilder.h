#ifndef INFANTRYBUILDER_H
#define INFANTRYBUILDER_H

#include "UnitBuilder.h"
#include "Infantry.h"

class InfantryBuilder : public UnitBuilder
{
public:
    std::unique_ptr<Unit> build() override;
};

#endif // INFANTRYBUILDER_H
