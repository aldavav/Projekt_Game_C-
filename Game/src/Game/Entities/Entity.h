#ifndef ENTITY_H
#define ENTITY_H

#include <Game/Entities/EntityTypes.h>
#include <Core/Logger/LoggerMacros.h>
#include <QPointF>
#include <fstream>
#include <string>
#include <memory>

class Player;

class Entity
{
protected:
    std::string m_name;

    EntityType m_type;

    std::string m_symbol;

    Player *m_owner;

    QPointF m_position;

    QPointF m_targetPosition;

    float m_speed = 200.0f;

public:
    Entity(EntityType type, const std::string &name, const std::string &symbol, Player *owner = nullptr);

    virtual ~Entity() = default;

    virtual void update(float deltaTime);

    QPointF getPosition() const { return m_position; }

    void setPosition(const QPointF &pos)
    {
        m_position = pos;
        m_targetPosition = pos;
    }

    void setTarget(const QPointF &target) { m_targetPosition = target; }

    EntityType getType() const { return m_type; }

    std::string getName() const { return m_name; }

    std::string getSymbol() const { return m_symbol; }

    Player *getOwner() const { return m_owner; }

    virtual void saveEntity(std::ofstream &file) = 0;

    static std::unique_ptr<Entity> createEntityFromFile(std::ifstream &file);
};

#endif
