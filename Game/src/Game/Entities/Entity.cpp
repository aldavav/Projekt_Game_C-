#include "Entity.h"

void Entity::update(float deltaTime)
{
    QPointF diff = m_targetPosition - m_position;

    float distSq = diff.x() * diff.x() + diff.y() * diff.y();

    if (distSq > Config::Entities::MIN_MOVE_DIST_SQ)
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
    Engine::Entity::Type type;
    if (!file.read(reinterpret_cast<char *>(&type), sizeof(type)))
    {
        return nullptr;
    }

    switch (type)
    {
    case Engine::Entity::Type::Unit:
        return nullptr;
    case Engine::Entity::Type::Building:
        return nullptr;
    case Engine::Entity::Type::Resource:
        return nullptr;
    case Engine::Entity::Type::Debris:
        return nullptr;
    default:
        return nullptr;
    }
}
