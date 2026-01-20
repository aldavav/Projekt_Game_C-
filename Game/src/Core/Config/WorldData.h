#ifndef WORLDDATA_H
#define WORLDDATA_H

#include <Core/Config/Configuration.h>
#include <cstdint>
#include <string>

namespace World
{
    enum class TileType : uint8_t
    {
        GRASS,
        DIRT,
        WATER,
        MOUNTAIN
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

    struct MapStats
    {
        int grassCount = 0;
        int waterCount = 0;
        int mountainCount = 0;
        int oreCount = 0;
        int dirtCount = 0;
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
