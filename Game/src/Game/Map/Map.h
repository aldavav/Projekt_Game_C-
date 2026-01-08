#ifndef MAP_H
#define MAP_H

#include <Logger/LoggerMacros.h>
#include <unordered_map>
#include "Chunk.h"
#include "Tile.h"
#include <string>
#include <vector>
#include <cmath>

enum Difficulty
{
    EASY,
    MEDIUM,
    HARD
};

class Map
{
private:
    Map() = default;

    ~Map();

    std::string m_mapName;

    Difficulty m_difficulty = EASY;

    std::unordered_map<uint64_t, Chunk *> m_chunks;

    uint64_t getChunkKey(int cx, int cy) const;

    void generateChunk(Chunk *chunk);

public:
    static Map &getInstance();

    void initializeNewMap(const std::string &name, Difficulty difficulty = EASY);

    Tile &getTileAt(int x, int y);

    bool isAreaWalkable(int x, int y, int w, int h);

    std::string getMapName() const { return m_mapName; }

    void clear();

    Map(const Map &) = delete;

    Map &operator=(const Map &) = delete;
};

#endif
