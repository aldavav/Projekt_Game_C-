#include "MapManager.h"

MapManager &MapManager::getInstance()
{
    static MapManager instance;
    return instance;
}

Tile &MapManager::getTileAt(int x, int y)
{
    int cx = std::floor(static_cast<float>(x) / Chunk::SIZE);
    int cy = std::floor(static_cast<float>(y) / Chunk::SIZE);

    uint64_t key = getChunkKey(cx, cy);

    if (m_chunks.find(key) == m_chunks.end())
    {
        Chunk *newChunk = new Chunk();
        newChunk->x = cx;
        newChunk->y = cy;
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

bool MapManager::isAreaWalkable(int x, int y, int width, int height)
{
    for (int i = x; i < x + width; ++i)
    {
        for (int j = y; j < y + height; ++j)
        {
            if (!getTileAt(i, j).walkable)
                return false;
        }
    }
    return true;
}

void MapManager::clear()
{
    for (auto &pair : m_chunks)
    {
        delete pair.second;
    }
    m_chunks.clear();
}
