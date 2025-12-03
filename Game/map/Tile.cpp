#include "Tile.h"

Tile::Tile(TerrainType type, const std::string &symbol, bool traversable, std::unique_ptr<Entity> entity)
    : m_type(type), m_symbol(symbol), m_isTraversable(traversable), m_entity(std::move(entity))
{
}

Tile::Tile(const Tile &other)
    : m_type(other.m_type), m_symbol(other.m_symbol), m_isTraversable(other.m_isTraversable)
{
    m_entity = nullptr;
    LOG_WARNING("Tile copy constructor called. Entity was deliberately reset to nullptr to prevent complex virtual cloning.");
}

Tile &Tile::operator=(const Tile &other)
{
    if (this != &other)
    {
        m_type = other.m_type;
        m_symbol = other.m_symbol;
        m_isTraversable = other.m_isTraversable;
        m_entity = nullptr;
    }
    return *this;
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

Entity *Tile::getEntity() const
{
    return m_entity.get();
}

void Tile::setEntity(std::unique_ptr<Entity> entity)
{
    m_entity = std::move(entity);
}

void Tile::saveTile(std::ofstream &file) const
{
    file.write(reinterpret_cast<const char *>(&m_type), sizeof(m_type));

    size_t symbolLength = m_symbol.size();
    file.write(reinterpret_cast<const char *>(&symbolLength), sizeof(size_t));
    file.write(m_symbol.c_str(), symbolLength);

    file.write(reinterpret_cast<const char *>(&m_isTraversable), sizeof(m_isTraversable));

    bool hasEntity = m_entity != nullptr;
    file.write(reinterpret_cast<const char *>(&hasEntity), sizeof(hasEntity));

    if (hasEntity)
    {
        m_entity->saveEntity(file);
    }
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

    bool hasEntity;
    file.read(reinterpret_cast<char *>(&hasEntity), sizeof(hasEntity));

    std::unique_ptr<Entity> loadedEntity = nullptr;
    if (hasEntity)
    {
        loadedEntity = Entity::createEntityFromFile(file);
    }

    return new Tile(type, symbol, traversable, std::move(loadedEntity));
}
