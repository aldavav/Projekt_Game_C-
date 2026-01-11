#ifndef MAP_H
#define MAP_H

#include <Core/Logger/LoggerMacros.h>
#include <Game/Map/Difficulty.h>
#include <Game/Map/MapStats.h>
#include <Game/Map/Chunk.h>
#include <Game/Map/Tile.h>
#include <unordered_map>
#include <cstdint>
#include <random>
#include <string>
#include <vector>
#include <cmath>

class Map
{
private:
    Map() = default;

    ~Map();

    std::string m_mapName;

    uint32_t m_seed = 12343;

    Difficulty m_difficulty = Difficulty::MEDIUM;

    std::unordered_map<uint64_t, Chunk *> m_chunks;

    uint64_t getChunkKey(int cx, int cy) const;

    void generateChunk(Chunk *chunk);

    MapStats m_stats;

public:
    static Map &getInstance();

    void initializeNewMap(const std::string &name, Difficulty difficulty = Difficulty::EASY);

    Tile &getTileAt(int x, int y);

    bool isAreaWalkable(int x, int y, int w, int h);

    void revealRadius(int centerQ, int centerR, int radius);

    std::string getMapName() const { return m_mapName; }

    void clear();

    Map(const Map &) = delete;

    Map &operator=(const Map &) = delete;

    const MapStats &getStats() const { return m_stats; }

    uint32_t getSeed() const { return m_seed; }
};

#endif
