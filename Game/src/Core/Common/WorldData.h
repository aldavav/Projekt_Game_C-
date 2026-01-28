#ifndef WORLDDATA_H
#define WORLDDATA_H

#include <Core/Config/Configuration.h>
#include <cstdint>
#include <string>

namespace World
{
    enum class TileType : uint8_t
    {
        Grass = 0,
        Dirt = 1,
        Water = 2,
        Mountain = 3
    };

    struct Tile
    {
        TileType type : 4;
        uint8_t variant : 4;
        bool discovered : 1;
        bool visible : 1;
        uint8_t padding : 6;
        Tile() : type(TileType::Grass), variant(0), discovered(false), visible(false), padding(0) {}
    };

    struct Chunk
    {
        static constexpr int Size = Config::World::CHUNK_SIZE;
        Tile tiles[Size][Size];
        int x = 0;
        int y = 0;
        void clearVisibility()
        {
            for (int i = 0; i < Size; ++i)
            {
                for (int j = 0; j < Size; ++j)
                {
                    tiles[i][j].visible = false;
                }
            }
        }
    };

    struct TileProperties
    {
        std::string name;
        float movementCost;
        bool walkable;
        bool buildable;
    };

    enum class MapType
    {
        ISLAND = 0,
        ARCHIPELAGO = 1,
        CONTINENTS = 2,
        PANGEA = 3
    };
}

#endif
