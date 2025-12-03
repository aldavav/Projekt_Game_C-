#ifndef TILE_H
#define TILE_H

#include <fstream>
#include <string>

class Player;

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

    Player *m_occupant;

public:
    Tile(TerrainType type, const std::string &symbol, bool traversable);

    ~Tile() = default;

    TerrainType getType() const;

    std::string getSymbol() const;

    bool isTraversable() const;

    Player *getOccupant() const;

    void setOccupant(Player *player);

    void saveTile(std::ofstream &file) const;

    static Tile* loadTile(std::ifstream &file);
};

#endif
