
#include "InfantryBuilder.h"

std::unique_ptr<Unit> InfantryBuilder::build()
{
    return std::make_unique<Infantry>(m_owner);
}

