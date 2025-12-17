#include "Entity.h"
#include "Player.h"

Entity::Entity(EntityType type,
               const std::string& name,
               const std::string& symbol,
               Player* owner)
    : m_name(name),
    m_type(type),
    m_symbol(symbol),
    m_owner(owner),
    m_x(0),
    m_y(0)
{
}

EntityType Entity::getType() const
{
    return m_type;
}

const std::string& Entity::getName() const
{
    return m_name;
}

const std::string& Entity::getSymbol() const
{
    return m_symbol;
}

Player* Entity::getOwner() const
{
    return m_owner;
}

int Entity::getX() const
{
    return m_x;
}

int Entity::getY() const
{
    return m_y;
}

void Entity::setPosition(int x, int y)
{
    m_x = x;
    m_y = y;
}

void Entity::setSelected(bool selected)
{
    m_selected = selected;
}

bool Entity::isSelected() const
{
    return m_selected;
}

std::unique_ptr<Entity> Entity::createEntityFromFile(std::ifstream& file)
{
    EntityType type;

    if (!file.read(reinterpret_cast<char*>(&type), sizeof(type)))
    {
        LOG_ERROR("Failed to read EntityType from file.");
        return nullptr;
    }

    switch (type)
    {
    case EntityType::UNIT:
        LOG_ERROR("Unit loading not implemented yet.");
        return nullptr;

    case EntityType::BUILDING:
        LOG_ERROR("Building loading not implemented yet.");
        return nullptr;

    case EntityType::RESOURCE:
        LOG_ERROR("Resource loading not implemented yet.");
        return nullptr;

    case EntityType::DEBRIS:
        LOG_ERROR("Debris loading not implemented yet.");
        return nullptr;

    default:
        LOG_ERROR("Unknown EntityType encountered during loading.");
        return nullptr;
    }
}
