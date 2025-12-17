#include "Infantry.h"

Infantry::Infantry(Player* owner)
    : Unit(
          UnitType::INFANTRY,
          "Infantry",
          "I",
          100,   // HP
          3,     // Move range
          owner)
{
}
