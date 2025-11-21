#ifndef UNITBUILDER_H
#define UNITBUILDER_H
#include "unit.h"
#include "customunit.h"
#include "QString"
class UnitBuilder
{
public:
    UnitBuilder &setUnitType(const QString & type);
    UnitBuilder &setUnitHealth(int health);
    UnitBuilder &setUnitDamage(int damage);
    UnitBuilder &setUnitMovementRange(int range);

    Unit * build()const;

    static Unit* createInfantry();
    static Unit* createTank();
    static Unit* createPlane();


private:
    QString unitType = "none";
    int unitHealth=100;
    int unitDamage=10;
    int unitRangeMovement=1;
};

#endif // UNITBUILDER_H
