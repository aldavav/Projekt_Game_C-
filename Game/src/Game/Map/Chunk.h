#ifndef CHUNK_H
#define CHUNK_H

#include "Tile.h"

struct Chunk
{
    static constexpr int SIZE = 32;

    Tile tiles[SIZE][SIZE];

    int x, y;
};

#endif
