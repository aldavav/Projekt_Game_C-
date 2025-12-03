#include "Tile.h"

Tile::Tile(TerrainType type, const std::string &symbol, bool traversable)
    : m_type(type), m_symbol(symbol), m_isTraversable(traversable), m_occupant(nullptr)
{
}

TerrainType Tile::getType() const
{
    return m_type;
}

std::string Tile::getSymbol() const
{
    return m_symbol;
}

bool Tile::isTraversable() const
{
    return m_isTraversable;
}

Player *Tile::getOccupant() const
{
    return m_occupant;
}

void Tile::setOccupant(Player *player)
{
    m_occupant = player;
}

void Tile::saveTile(std::ofstream &file) const
{
    file.write(reinterpret_cast<const char *>(&m_type), sizeof(m_type));

    size_t symbolLength = m_symbol.size();
    file.write(reinterpret_cast<const char *>(&symbolLength), sizeof(size_t));
    file.write(m_symbol.c_str(), symbolLength);

    file.write(reinterpret_cast<const char *>(&m_isTraversable), sizeof(m_isTraversable));
}

Tile *Tile::loadTile(std::ifstream &file)
{
    TerrainType type;
    file.read(reinterpret_cast<char *>(&type), sizeof(type));

    size_t symbolLength;
    file.read(reinterpret_cast<char *>(&symbolLength), sizeof(size_t));
    std::string symbol(symbolLength, '\0');
    file.read(&symbol[0], symbolLength);

    bool traversable;
    file.read(reinterpret_cast<char *>(&traversable), sizeof(traversable));

    return new Tile(type, symbol, traversable);
}
