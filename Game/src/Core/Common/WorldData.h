#ifndef WORLDDATA_H
#define WORLDDATA_H

#include <Core/Config/Configuration.h>
#include <cstdint>
#include <string>

namespace World
{
    enum class TileType : uint8_t
    {
        GRASS = 0,
        DIRT = 1,
        WATER = 2,
        MOUNTAIN = 3
    };

    struct Tile
    {
        TileType type = TileType::GRASS;
        uint8_t variant = 0;
        bool discovered = false;
    };

    struct Chunk
    {
        static constexpr int S = Config::World::CHUNK_SIZE;
        Tile tiles[S][S];
        int x, y;
    };

    struct TileProperties
    {
        std::string name;
        bool walkable;
        bool buildable;
        float movementCost;
    };
}

#endif
