#include "Entity.h"

void Entity::update(float deltaTime)
{
    QPointF diff = m_targetPosition - m_position;

    float distSq = diff.x() * diff.x() + diff.y() * diff.y();

    if (distSq > GameConfig::Entities::MIN_MOVE_DISTANCE_SQ)
    {
        float dist = std::sqrt(distSq);
        QPointF direction = diff / dist;

        float moveStep = m_speed * deltaTime;

        if (moveStep > dist)
            moveStep = dist;

        m_position += direction * moveStep;
    }
}

void Entity::setPosition(const QPointF &pos)
{
    m_position = pos;
    m_targetPosition = pos;
}

std::unique_ptr<Entity> Entity::createEntityFromFile(std::ifstream &file)
{
    EntityTypes::EntityType type;
    if (!file.read(reinterpret_cast<char *>(&type), sizeof(type)))
    {
        return nullptr;
    }

    switch (type)
    {
    case EntityTypes::EntityType::UNIT:
        return nullptr;
    case EntityTypes::EntityType::BUILDING:
        return nullptr;
    case EntityTypes::EntityType::RESOURCE:
        return nullptr;
    case EntityTypes::EntityType::DEBRIS:
        return nullptr;
    default:
        return nullptr;
    }
}
