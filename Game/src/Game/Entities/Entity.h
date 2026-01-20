#ifndef ENTITY_H
#define ENTITY_H

#include <Core/Config/EntityTypes.h>
#include <Core/Config/GameConfig.h>
#include <fstream>
#include <QPointF>
#include <string>
#include <memory>

class Player;

class Entity
{
public:
    Entity() = default;

    virtual ~Entity() = default;

    virtual void update(float deltaTime);

    QPointF getPosition() const { return m_position; }

    QPointF getTarget() const { return m_targetPosition; }

    EntityTypes::EntityType getType() const { return m_type; }

    std::string getName() const { return m_name; }

    std::string getSymbol() const { return m_symbol; }

    Player *getOwner() const { return m_owner; }

    void setPosition(const QPointF &pos);

    void setTarget(const QPointF &target) { m_targetPosition = target; }

    void setOwner(Player *owner) { m_owner = owner; }

    virtual void saveEntity(std::ofstream &file) = 0;

    static std::unique_ptr<Entity> createEntityFromFile(std::ifstream &file);

protected:
    std::string m_name;

    std::string m_symbol;

    EntityTypes::EntityType m_type = EntityTypes::EntityType::UNIT;

    Player *m_owner = nullptr;

    QPointF m_position = {0.0f, 0.0f};

    QPointF m_targetPosition = {0.0f, 0.0f};

    float m_speed = GameConfig::Entities::DEFAULT_SPEED;
};

#endif
