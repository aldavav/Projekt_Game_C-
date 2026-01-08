#include "Map.h"

Map &Map::getInstance()
{
    static Map instance;
    return instance;
}

Map::~Map()
{
    clear();
}

void Map::initializeNewMap(const std::string &name, Difficulty difficulty)
{
    clear();
    m_mapName = name;
    m_difficulty = difficulty;
    LOG_INFO("RTS World Initialized: " + name);
}

uint64_t Map::getChunkKey(int cx, int cy) const
{
    return (static_cast<uint64_t>(cx) << 32) | (static_cast<uint32_t>(cy));
}

Tile &Map::getTileAt(int x, int y)
{
    int cx = static_cast<int>(std::floor(static_cast<float>(x) / Chunk::SIZE));
    int cy = static_cast<int>(std::floor(static_cast<float>(y) / Chunk::SIZE));

    uint64_t key = getChunkKey(cx, cy);

    if (m_chunks.find(key) == m_chunks.end())
    {
        Chunk *newChunk = new Chunk();
        newChunk->x = cx;
        newChunk->y = cy;
        generateChunk(newChunk);
        m_chunks[key] = newChunk;
    }

    int tx = x % Chunk::SIZE;
    int ty = y % Chunk::SIZE;
    if (tx < 0)
        tx += Chunk::SIZE;
    if (ty < 0)
        ty += Chunk::SIZE;

    return m_chunks[key]->tiles[tx][ty];
}

void Map::generateChunk(Chunk *chunk)
{
    for (int y = 0; y < Chunk::SIZE; ++y)
    {
        for (int x = 0; x < Chunk::SIZE; ++x)
        {
            chunk->tiles[x][y].type = TileType::GRASS;
            chunk->tiles[x][y].walkable = true;
        }
    }
}

void Map::clear()
{
    for (auto &pair : m_chunks)
    {
        delete pair.second;
    }
    m_chunks.clear();
}

bool Map::isAreaWalkable(int x, int y, int w, int h)
{
    for (int i = x; i < x + w; ++i)
    {
        for (int j = y; j < y + h; ++j)
        {
            if (!getTileAt(i, j).walkable)
                return false;
        }
    }
    return true;
}
