#include "Unit.h"

Unit::Unit(UnitType unitType,
           const std::string& name,
           const std::string& symbol,
           int maxHp,
           int moveRange,
           Player* owner)
    : Entity(EntityType::UNIT, name, symbol, owner),
    m_unitType(unitType),
    m_hp(maxHp),
    m_maxHp(maxHp),
    m_moveRange(moveRange)

{

}

UnitType Unit::getUnitType() const
{
    return m_unitType;
}

int Unit::getHp() const
{
    return m_hp;
}

int Unit::getMaxHp() const
{
    return m_maxHp;
}

int Unit::getMoveRange() const
{
    return m_moveRange;
}

bool Unit::canMoveTo(int x, int y) const
{
    int dx = std::abs(x - m_x);
    int dy = std::abs(y - m_y);
    return (dx + dy) <= m_moveRange;
}

void Unit::moveTo(int x, int y)
{
    if (!canMoveTo(x, y))
        return;

    setPosition(x, y);
}

void Unit::saveEntity(std::ofstream& file)
{
    file.write(reinterpret_cast<const char*>(&m_type), sizeof(m_type));
    file.write(reinterpret_cast<const char*>(&m_unitType), sizeof(m_unitType));
    file.write(reinterpret_cast<const char*>(&m_hp), sizeof(m_hp));
    file.write(reinterpret_cast<const char*>(&m_maxHp), sizeof(m_maxHp));
    file.write(reinterpret_cast<const char*>(&m_moveRange), sizeof(m_moveRange));
}
