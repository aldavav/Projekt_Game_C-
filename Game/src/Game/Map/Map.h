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

    void initializeNewMap(const std::string &name, Engine::Difficulty difficulty, uint32_t seed, int type);

    void clear();

    World::Tile &getTileAt(int x, int y);

    bool isAreaWalkable(int x, int y, int w, int h);

    void revealRadiusWithCleanup(int centerQ, int centerR, int radius);

    void clearAllVisible();

    bool hasTileAt(int q, int r);

    std::string getMapName() const { return m_mapName; }

    uint32_t getSeed() const { return m_seed; }

    void debugRevealAll();

    std::unordered_map<uint64_t, World::Chunk *> getChunks() { return m_chunks; }

private:
    Map() = default;

    uint64_t getChunkKey(int cx, int cy) const;

    void generateChunk(World::Chunk *chunk);

    std::string m_mapName = Config::Gameplay::DEFAULT_MISSION_NAME.toStdString();

    uint32_t m_seed = Config::World::DEFAULT_SEED;

    Engine::Difficulty m_difficulty = Config::Gameplay::DEFAULT_DIFFICULTY;

    std::unordered_map<uint64_t, World::Chunk *> m_chunks;

    World::MapType m_mapType = World::MapType::ISLAND;
};

#endif
