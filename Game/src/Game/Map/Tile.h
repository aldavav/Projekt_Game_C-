#ifndef TILE_H
#define TILE_H

#include <cstdint>

enum class TileType : uint8_t
{
    GRASS,
    DIRT,
    WATER,
    MOUNTAIN,
    ORE_DEPOSIT
};

struct Tile
{
    TileType type = TileType::GRASS;

    bool walkable = true;

    bool buildable = true;
    
    uint8_t variant = 0;
};

#endif
