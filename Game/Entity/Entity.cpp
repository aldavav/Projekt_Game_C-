#include "Entity.h"

Entity::Entity(EntityType type, const std::string &name, const std::string &symbol, Player *owner)
    : m_name(name), m_type(type), m_symbol(symbol), m_owner(owner), m_x(0), m_y(0)
{
}

EntityType Entity::getType() const
{
    return m_type;
}

std::string Entity::getName() const
{
    return m_name;
}

std::string Entity::getSymbol() const
{
    return m_symbol;
}

Player *Entity::getOwner() const
{
    return m_owner;
}

void Entity::setPosition(int x, int y)
{
    m_x = x;
    m_y = y;
}

std::unique_ptr<Entity> Entity::createEntityFromFile(std::ifstream &file)
{
    EntityType type;

    if (!file.read(reinterpret_cast<char *>(&type), sizeof(type)))
    {
        LOG_ERROR("Failed to read EntityType from file.");
        return nullptr;
    }

    switch (type)
    {
    case EntityType::UNIT:
        LOG_ERROR("Entity loading not yet implemented for UNIT type.");
        return nullptr;
    case EntityType::BUILDING:
        LOG_ERROR("Entity loading not yet implemented for BUILDING type.");
        return nullptr;
    case EntityType::RESOURCE:
        LOG_ERROR("Entity loading not yet implemented for RESOURCE type.");
        return nullptr;
    case EntityType::DEBRIS:
        LOG_ERROR("Entity loading not yet implemented for DEBRIS type.");
        return nullptr;
    default:
        LOG_ERROR("Unknown EntityType encountered during loading.");
        return nullptr;
    }
}
