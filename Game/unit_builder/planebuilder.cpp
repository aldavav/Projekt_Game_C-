#include "PlaneBuilder.h"

std::unique_ptr<Unit> PlaneBuilder::build()
{
    return std::make_unique<Plane>(m_owner);
}

