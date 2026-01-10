#include "Map.h"

Map &Map::getInstance()
{
    static Map instance;
    return instance;
}

Map::~Map()
{
    clear();
}

void Map::initializeNewMap(const std::string &name, Difficulty difficulty)
{
    clear();
    m_mapName = name;
    m_difficulty = difficulty;
    LOG_INFO("RTS World Initialized: " + name);
}

uint64_t Map::getChunkKey(int cx, int cy) const
{
    uint32_t low = static_cast<uint32_t>(cx);
    uint32_t high = static_cast<uint32_t>(cy);
    return (static_cast<uint64_t>(high) << 32) | low;
}

Tile &Map::getTileAt(int x, int y)
{
    int cx = static_cast<int>(std::floor(static_cast<float>(x) / Chunk::SIZE));
    int cy = static_cast<int>(std::floor(static_cast<float>(y) / Chunk::SIZE));

    uint64_t key = getChunkKey(cx, cy);

    if (m_chunks.find(key) == m_chunks.end())
    {
        Chunk *newChunk = new Chunk();
        newChunk->x = cx;
        newChunk->y = cy;
        generateChunk(newChunk);
        m_chunks[key] = newChunk;
    }

    int tx = x % Chunk::SIZE;
    int ty = y % Chunk::SIZE;
    if (tx < 0)
        tx += Chunk::SIZE;
    if (ty < 0)
        ty += Chunk::SIZE;

    return m_chunks[key]->tiles[tx][ty];
}

#include <cmath>

float getNoise(float q, float r, uint32_t seed)
{
    auto hash = [&](int x, int y)
    {
        unsigned int h = seed ^ (x * 1327) ^ (y * 9743);
        h = (h ^ (h >> 16)) * 0x45d9f3b;
        h = (h ^ (h >> 16)) * 0x45d9f3b;
        h = h ^ (h >> 16);
        return h / (float)0xFFFFFFFF;
    };

    int iq = std::floor(q);
    int ir = std::floor(r);
    float fq = q - iq;
    float fr = r - ir;

    float v1 = hash(iq, ir);
    float v2 = hash(iq + 1, ir);
    float v3 = hash(iq, ir + 1);
    float v4 = hash(iq + 1, ir + 1);

    float top = v1 * (1 - fq) + v2 * fq;
    float bot = v3 * (1 - fq) + v4 * fq;
    return top * (1 - fr) + bot * fr;
}

void Map::generateChunk(Chunk *chunk)
{
    const int MAP_RADIUS = 40;
    const float scale = 15.0f;

    for (int ty = 0; ty < Chunk::SIZE; ++ty)
    {
        for (int tx = 0; tx < Chunk::SIZE; ++tx)
        {
            int q = chunk->x * Chunk::SIZE + tx;
            int r = chunk->y * Chunk::SIZE + ty;

            float e = 1.0f * getNoise(q / scale, r / scale, m_seed);
            e += 0.5f * getNoise(q / (scale / 2), r / (scale / 2), m_seed);
            e += 0.25f * getNoise(q / (scale / 4), r / (scale / 4), m_seed);
            e /= (1.0f + 0.5f + 0.25f);

            float dist = (std::abs(q) + std::abs(q + r) + std::abs(r)) / 2.0f;
            float d = dist / MAP_RADIUS;

            float height = (e + 0.1f) - (d * d);

            if (height < 0.2f)
            {
                chunk->tiles[tx][ty].type = TileType::WATER;
            }
            else if (height < 0.3f)
            {
                chunk->tiles[tx][ty].type = TileType::DIRT;
            }
            else if (height < 0.65f)
            {
                chunk->tiles[tx][ty].type = TileType::GRASS;
            }
            else
            {

                float oreChance = (float)rand() / (float)RAND_MAX;
                if (height > 0.85f || (height > 0.7f && oreChance > 0.85f))
                {
                    chunk->tiles[tx][ty].type = TileType::ORE_DEPOSIT;
                }
                else
                {
                    chunk->tiles[tx][ty].type = TileType::MOUNTAIN;
                }
            }

            switch (chunk->tiles[tx][ty].type)
            {
            case TileType::GRASS:
                m_stats.grassCount++;
                break;
            case TileType::WATER:
                m_stats.waterCount++;
                break;
            case TileType::MOUNTAIN:
                m_stats.mountainCount++;
                break;
            case TileType::ORE_DEPOSIT:
                m_stats.oreCount++;
                break;
            case TileType::DIRT:
                m_stats.dirtCount++;
                break;
            }
        }
    }
}

void Map::clear()
{
    for (auto &pair : m_chunks)
    {
        delete pair.second;
    }
    m_chunks.clear();
}

bool Map::isAreaWalkable(int q, int r, int w, int h)
{
    for (int i = q; i < q + w; ++i)
    {
        for (int j = r; j < r + h; ++j)
        {
            TileType type = getTileAt(i, j).type;

            if (type == TileType::WATER || type == TileType::MOUNTAIN)
            {
                return false;
            }
        }
    }
    return true;
}

void Map::revealRadius(int centerQ, int centerR, int radius) {
    for (int q = centerQ - radius; q <= centerQ + radius; ++q) {
        for (int r = centerR - radius; r <= centerR + radius; ++r) {
            int dist = (std::abs(centerQ - q) + std::abs(centerQ + centerR - q - r) + std::abs(centerR - r)) / 2;
            if (dist <= radius) {
                getTileAt(q, r).discovered = true;
            }
        }
    }
}
