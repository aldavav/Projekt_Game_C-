#ifndef PLANE_H
#define PLANE_H
#include "unit_builder/Unit.h"
class Plane : public Unit
{
public:
    explicit Plane(Player* owner);
    void update(float deltaTime) override;
};
#endif // PLANE_H
