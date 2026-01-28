#ifndef ENTITY_H
#define ENTITY_H

#include <Core/Config/Configuration.h>
#include <fstream>
#include <QPointF>
#include <string>
#include <memory>

class Player;

enum class Team { Player, Enemy, Neutral };

class Entity
{
public:
    Entity() = default;
    virtual ~Entity() = default;

    virtual void update(float deltaTime);

    // Get
    QPointF getPosition() const { return m_position; }
    QPointF getTarget() const { return m_targetPosition; }
    Engine::Entity::Type getType() const { return m_type; }
    std::string getName() const { return m_name; }
    std::string getSymbol() const { return m_symbol; }
    Player *getOwner() const { return m_owner; }
    Team getTeam() const { return m_team; }
    int getHealth() const { return m_health; }
    int getMaxHealth() const { return m_maxHealth; }
    bool isAlive() const { return m_health > 0; }

    // Set
    void setPosition(const QPointF &pos);
    void setTarget(const QPointF &target) { m_targetPosition = target; }
    void setOwner(Player *owner) { m_owner = owner; }
    void setName(const std::string &name) { m_name = name; }
    void setSymbol(const std::string &symbol) { m_symbol = symbol; }
    void setType(Engine::Entity::Type type) { m_type = type; }
    void setSpeed(float speed) { m_speed = speed; }
    void setTeam(Team team) { m_team = team; }
    void setHealth(int health) { m_health = health; }
    void setMaxHealth(int maxHealth) { m_maxHealth = maxHealth; }

    void setAttackPower(int power) { m_attackPower = power; }

    void takeDamage(int amount) { m_health = std::max(0, m_health - amount); }

    virtual void saveEntity(std::ofstream &file){};
    static std::unique_ptr<Entity> createEntityFromFile(std::ifstream &file);

protected:
    std::string m_name;
    std::string m_symbol;
    Engine::Entity::Type m_type = Engine::Entity::Type::Unit;
    Player *m_owner = nullptr;
    QPointF m_position = {0.0f, 0.0f};
    QPointF m_targetPosition = {0.0f, 0.0f};

    float m_speed = Config::Entities::DEFAULT_SPEED;
    float m_productionTimer = 0.0f;

    Team m_team = Team::Player;
    int m_health = 100;
    int m_maxHealth = 100;
    int m_attackPower = 10;
};

#endif
