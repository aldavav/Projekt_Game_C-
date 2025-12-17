#include "TankBuilder.h"

std::unique_ptr<Unit> TankBuilder::build()
{
    return std::make_unique<Tank>(m_owner);
}
