#ifndef MAP_H
#define MAP_H

#include <Core/Common/GameTypes.h>
#include <Core/Common/WorldData.h>
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

    void initializeNewMap(const std::string &name = Config::World::DEFAULT_MAP_NAME.toStdString(), Engine::Difficulty difficulty = Config::Gameplay::DEFAULT_DIFFICULTY);

    void clear();

    World::Tile &getTileAt(int x, int y);

    bool isAreaWalkable(int x, int y, int w, int h);

    void revealRadius(int centerQ, int centerR, int radius);

    std::string getMapName() const { return m_mapName; }

    uint32_t getSeed() const { return m_seed; }

    const World::MapStats &getStats() const { return m_stats; }

    void debugRevealAll();

private:
    Map() = default;

    ~Map() = default;

    uint64_t getChunkKey(int cx, int cy) const;

    void generateChunk(World::Chunk *chunk);

    std::string m_mapName = Config::World::DEFAULT_MAP_NAME.toStdString();

    uint32_t m_seed = Config::World::DEFAULT_SEED;

    Engine::Difficulty m_difficulty = Config::Gameplay::DEFAULT_DIFFICULTY;

    World::MapStats m_stats;

    std::unordered_map<uint64_t, World::Chunk *> m_chunks;
};

#endif
