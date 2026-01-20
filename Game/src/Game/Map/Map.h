#ifndef MAP_H
#define MAP_H

#include <Core/Config/GameConfig.h>
#include <Core/Config/Difficulty.h>
#include <Core/Config/MapStats.h>
#include <Core/Config/Chunk.h>
#include <Core/Config/Tile.h>
#include <unordered_map>
#include <cstdint>
#include <string>
#include <vector>
#include <random>

class Map
{
public:
    static Map &getInstance();

    Map(const Map &) = delete;

    Map &operator=(const Map &) = delete;

    void initializeNewMap(const std::string &name = GameConfig::World::DEFAULT_MAP_NAME.toStdString(), Difficulty difficulty = GameConfig::DEFAULT_DIFFICULTY);

    void clear();

    Tile &getTileAt(int x, int y);

    bool isAreaWalkable(int x, int y, int w, int h);

    void revealRadius(int centerQ, int centerR, int radius);

    std::string getMapName() const { return m_mapName; }

    uint32_t getSeed() const { return m_seed; }

    const MapStats &getStats() const { return m_stats; }

    void debugRevealAll();

private:
    Map() = default;

    ~Map() = default;

    uint64_t getChunkKey(int cx, int cy) const;

    void generateChunk(Chunk *chunk);

    std::string m_mapName = GameConfig::World::DEFAULT_MAP_NAME.toStdString();

    uint32_t m_seed = GameConfig::World::DEFAULT_SEED;

    Difficulty m_difficulty = GameConfig::World::DEFAULT_DIFFICULTY;

    MapStats m_stats;

    std::unordered_map<uint64_t, Chunk *> m_chunks;
};

#endif
