#include "Tank.h"

Tank::Tank(Player* owner)
    : Unit(
          UnitType::TANK,
          "Tank",
          "T",
          300,  // HP
          2,    // Move range
          owner)
{
}
