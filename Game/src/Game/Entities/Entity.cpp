#include "Entity.h"
#include <game/engine/GameManager.h>
#include <cmath>
#include <algorithm>

void Entity::update(float deltaTime)
{
    // LOGIKA POHYBU
    if (m_type == Engine::Entity::Type::Unit)
    {
        QPointF diff = m_targetPosition - m_position;
        float distSq = diff.x() * diff.x() + diff.y() * diff.y();

        if (distSq > 0.001f)
        {
            float dist = std::sqrt(distSq);
            float moveStep = m_speed * deltaTime;

            if (moveStep > dist) moveStep = dist;
            m_position += (diff / dist) * moveStep;
        }
    }

    // LOGIKA BUDOVY
    if (m_type == Engine::Entity::Type::Building)
    {
        m_productionTimer += deltaTime;
        if (m_productionTimer >= 1.0f)
        {
            if (m_team == Team::Player) {
                GameManager::getInstance().addGold(10);
            }
            m_productionTimer = 0.0f;
        }
    }

    // AI ENEMY
    if (m_team == Team::Enemy && m_type == Engine::Entity::Type::Unit)
    {
        Entity* closestTarget = nullptr;
        float minTargetDistSq = 1000000.0f;

        auto& allEntities = GameManager::getInstance().getEntities();
        for (auto& other : allEntities) {
            if (other && other.get() != this && other->getTeam() == Team::Player) {
                QPointF targetDiff = other->getPosition() - m_position;
                float dSq = targetDiff.x() * targetDiff.x() + targetDiff.y() * targetDiff.y();
                if (dSq < minTargetDistSq) {
                    minTargetDistSq = dSq;
                    closestTarget = other.get();
                }
            }
        }

        if (closestTarget) {
            // Nepřítel pronásleduje hráče
            setTarget(closestTarget->getPosition());

            // Útok (1.44 = 1.2 hexu)
            if (minTargetDistSq < 1.44f) {
                closestTarget->takeDamage(static_cast<int>(m_attackPower * deltaTime + 0.5f));
            }
        }
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

    return nullptr;
}
