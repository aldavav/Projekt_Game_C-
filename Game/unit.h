#ifndef UNIT_H
#define UNIT_H

#include "Entity.h"
#include "Player.h"
#include <string>
enum class UnitType
{   NONE,
    INFANTRY,
    TANK,
    PLANE,
    // případně další typy jednotek
};
class Unit : public Entity
{
private:
    UnitType m_unitType;  // správný typ tady

    int m_hp;
    int m_maxHp;
    int m_moveRange;

public:
    Unit(UnitType unitType,
         const std::string& name,
         const std::string& symbol,
         int maxHp,
         int moveRange,
         Player* owner = nullptr);
    UnitType getUnitType() const;  // vrací UnitType, ne EntityType

    int getHp() const;
    int getMaxHp() const;
    int getMoveRange() const;

    bool canMoveTo(int x, int y) const;
    void moveTo(int x, int y);

    void saveEntity(std::ofstream& file) override;

};

#endif
