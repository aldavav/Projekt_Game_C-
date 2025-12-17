#ifndef PLANE_H
#define PLANE_H
#include "Unit.h"
class Plane : public Unit
{
public:
    explicit Plane(Player* owner);
    void update(float deltaTime) override;
};
#endif // PLANE_H
