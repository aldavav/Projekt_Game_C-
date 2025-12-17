#ifndef PLANEBUILDER_H
#define PLANEBUILDER_H

#include "UnitBuilder.h"
#include "Plane.h"

class PlaneBuilder : public UnitBuilder
{
public:
    std::unique_ptr<Unit> build() override;
};

#endif // PLANEBUILDER_H
