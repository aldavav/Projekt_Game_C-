#ifndef ENTITY_H
#define ENTITY_H

#include <Core/Logger/LoggerMacros.h>
#include <fstream>
#include <string>
#include <memory>

class Location;
class Tile;

enum EntityType
{
    UNIT = 1,
    BUILDING = 2,
    RESOURCE = 3,
    DEBRIS = 4
};

class Player;

class Entity
{
private:
    std::string m_name;

    EntityType m_type;

    std::string m_symbol;

    Player *m_owner;

    int m_x, m_y;

public:
    Entity(EntityType type, const std::string &name, const std::string &symbol, Player *owner = nullptr);

    virtual ~Entity() = default;

    EntityType getType() const;

    std::string getName() const;

    std::string getSymbol() const;

    Player *getOwner() const;

    int getX() const { return m_x; }

    int getY() const { return m_y; }

    void setPosition(int x, int y);

    virtual void saveEntity(std::ofstream &file) = 0;

    static std::unique_ptr<Entity> createEntityFromFile(std::ifstream &file);
};

#endif
