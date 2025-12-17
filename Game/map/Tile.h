#ifndef TILE_H
#define TILE_H

#include "../Entity/Entity.h"
#include <LoggerMacros.h>
#include <fstream>
#include <string>
#include <memory>

enum TerrainType
{
    GRASS,
    SAND,
    WATER,
    ROCK
};

class Tile
{
private:
    TerrainType m_type;

    std::string m_symbol;

    bool m_isTraversable;

    std::unique_ptr<Entity> m_entity;

public:
    Tile(TerrainType type, const std::string &symbol, bool traversable, std::unique_ptr<Entity> entity = nullptr);

    Tile(const Tile &other);

    Tile &operator=(const Tile &other);

    ~Tile() = default;

    TerrainType getType() const;

    std::string getSymbol() const;

    bool isTraversable() const;

    Entity *getEntity() const;

    void setEntity(std::unique_ptr<Entity> entity);

    void saveTile(std::ofstream &file) const;

    static Tile *loadTile(std::ifstream &file);

    std::unique_ptr<Entity> takeEntity();

};

#endif
