#ifndef CUSTOMUNIT_H
#define CUSTOMUNIT_H
#include "unit.h"
class CustomUnit:public Unit
{
public:
    CustomUnit(const QString& type, int health, int damage, int movement);


    QString getType() const override { return unit_type; }

};

#endif // CUSTOMUNIT_H
