#ifndef ENTITY_H
#define ENTITY_H

#include <Core/Config/Configuration.h>
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

    Engine::Entity::Type getType() const { return m_type; }

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

    Engine::Entity::Type m_type = Engine::Entity::Type::UNIT;

    Player *m_owner = nullptr;

    QPointF m_position = {0.0f, 0.0f};

    QPointF m_targetPosition = {0.0f, 0.0f};

    float m_speed = Config::Entities::DEFAULT_SPEED;
};

#endif
