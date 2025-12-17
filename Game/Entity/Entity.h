#ifndef ENTITY_H
#define ENTITY_H

#include <LoggerMacros.h>
#include <fstream>
#include <string>
#include <memory>

class Player;

enum class EntityType
{
    UNIT = 1,
    BUILDING = 2,
    RESOURCE = 3,
    DEBRIS = 4
};


class Entity
{
protected:
    std::string m_name;
    EntityType m_type;
    std::string m_symbol;
    Player* m_owner;

    int m_x;
    int m_y;

    bool m_selected = false;

public:
    Entity(EntityType type,
           const std::string& name,
           const std::string& symbol,
           Player* owner = nullptr);

    virtual ~Entity() = default;

    // ---- INFO ----
    EntityType getType() const;
    const std::string& getName() const;
    const std::string& getSymbol() const;
    Player* getOwner() const;

    // ---- POSITION ----
    int getX() const;
    int getY() const;
    void setPosition(int x, int y);

    // ---- SELECTION ----
    void setSelected(bool selected);
    bool isSelected() const;

    // ---- UPDATE ----
    virtual void update(float deltaTime) {}

    // ---- SAVE / LOAD ----
    virtual void saveEntity(std::ofstream& file) = 0;
    static std::unique_ptr<Entity> createEntityFromFile(std::ifstream& file);
};

#endif
