#include "unit.h"

Unit::Unit(const QString& m_unit_type,int m_unit_health,int m_unit_damage,int m_unit_range_movement)
    :unit_type(m_unit_type),
    unit_health(m_unit_health),
    unit_damage(m_unit_damage),
    unit_range_movement(m_unit_range_movement)

{}


int Unit::getUnitHealth(){
    return unit_health;
}

int Unit::getUnitDamage(){
    return unit_damage;
}

int Unit::getMovementRange(){
    return unit_range_movement;
}
