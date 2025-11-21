#include "unitbuilder.h"

UnitBuilder& UnitBuilder::setUnitType(const QString& type)
{
    unitType = type;
    return *this;
}

UnitBuilder& UnitBuilder::setUnitHealth(int health)
{
    unitHealth = health;
    return *this;
}

UnitBuilder& UnitBuilder::setUnitDamage(int damage)
{
    unitDamage = damage;
    return *this;
}

UnitBuilder& UnitBuilder::setUnitMovementRange(int range)
{
    unitRangeMovement = range;
    return *this;
}

Unit* UnitBuilder::build() const
{
    return new CustomUnit(unitType, unitHealth, unitDamage, unitRangeMovement);
}
