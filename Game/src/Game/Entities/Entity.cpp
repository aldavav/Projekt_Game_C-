#include "Entity.h"

Entity::Entity(EntityTypes::EntityType type, const std::string &name, const std::string &symbol, Player *owner)
    : m_name(name), m_type(type), m_symbol(symbol), m_owner(owner),
      m_position(0, 0), m_targetPosition(0, 0)
{
}

void Entity::update(float deltaTime)
{
    QPointF diff = m_targetPosition - m_position;
    float distSq = diff.x() * diff.x() + diff.y() * diff.y();

    if (distSq > 1.0f)
    {
        float dist = std::sqrt(distSq);
        QPointF direction = diff / dist;

        float moveStep = m_speed * deltaTime;
        if (moveStep > dist)
            moveStep = dist;

        m_position += direction * moveStep;
    }
}

std::unique_ptr<Entity> Entity::createEntityFromFile(std::ifstream &file)
{
    EntityTypes::EntityType type;
    if (!file.read(reinterpret_cast<char *>(&type), sizeof(type)))
    {
        LOG_ERROR("Failed to read EntityType from file.");
        return nullptr;
    }

    switch (type)
    {
        case EntityTypes::EntityType::UNIT:
            LOG_ERROR("Entity loading not yet implemented for UNIT type.");
            return nullptr;
    case EntityTypes::EntityType::BUILDING:
        LOG_ERROR("Entity loading not yet implemented for BUILDING type.");
        return nullptr;
    case EntityTypes::EntityType::RESOURCE:
        LOG_ERROR("Entity loading not yet implemented for RESOURCE type.");
        return nullptr;
    case EntityTypes::EntityType::DEBRIS:
        LOG_ERROR("Entity loading not yet implemented for DEBRIS type.");
        return nullptr;
    default:
        LOG_ERROR("Unknown EntityType encountered during loading.");
        return nullptr;
    }
}
