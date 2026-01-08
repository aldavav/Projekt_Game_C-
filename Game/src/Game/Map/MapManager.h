#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include <unordered_map>
#include "Chunk.h"
#include <vector>
#include <string>
#include <cmath>

class MapManager
{
public:
    static MapManager &getInstance();

    Tile &getTileAt(int x, int y);

    bool isAreaWalkable(int x, int y, int width, int height);

    void clear();

private:
    MapManager() = default;

    ~MapManager() = default;

    uint64_t getChunkKey(int cx, int cy) const
    {
        return (static_cast<uint64_t>(cx) << 32) | (static_cast<uint32_t>(cy));
    }

    std::unordered_map<uint64_t, Chunk *> m_chunks;
};

#endif
