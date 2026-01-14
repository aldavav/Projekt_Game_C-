#ifndef CHUNK_H
#define CHUNK_H

#include <Game/Map/Tile.h>

struct Chunk
{
    static constexpr int SIZE = 32;

    Tile tiles[SIZE][SIZE];

    int x, y;
};

#endif
