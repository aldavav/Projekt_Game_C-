#ifndef UNIT_H
#define UNIT_H

#include <QString>

class Unit
{
public:
    Unit(const QString& m_unit_type,int m_unit_health,int m_unit_damage,int m_unit_range_movement);

    virtual ~Unit()=default;

    virtual QString getType() const=0;

    int getUnitHealth();
    int getUnitDamage();
    int getMovementRange();

protected:
    QString unit_type;
    int unit_health;
    int unit_damage;
    int unit_range_movement;

private:


};

#endif // UNIT_H
