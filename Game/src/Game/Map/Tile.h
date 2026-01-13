#ifndef TILE_H
#define TILE_H

#include <cstdint>
#include <string>

enum class TileType : uint8_t
{
    GRASS,
    DIRT,
    WATER,
    MOUNTAIN
};

struct TileProperties
{
    std::string name;
    bool walkable;
    bool buildable;
    float movementCost;
};

struct Tile
{
    TileType type = TileType::GRASS;
    uint8_t variant = 0;
    bool discovered = false;
};

#endif
