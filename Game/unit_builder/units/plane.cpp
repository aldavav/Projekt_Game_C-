#include "Plane.h"

Plane::Plane(Player* owner)
    : Unit(UnitType::PLANE, "Plane", "P", 100, 5, owner)
{
}

void Plane::update(float deltaTime)
{
    // Můžeš sem dát specifickou logiku pohybu letadla
    Unit::update(deltaTime);
}
