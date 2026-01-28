#include "Map.h"

Map &Map::getInstance()
{
    static Map instance;
    return instance;
}

void Map::initializeNewMap(const std::string &name, Engine::Difficulty difficulty, uint32_t seed, int type)
{
    clear();
    m_mapName = name;
    m_difficulty = difficulty;
    m_seed = seed;
    m_mapType = static_cast<World::MapType>(type);

    int startRadius = Config::World::DEFAULT_DISCOVER_RADIUS;
    for (int q = -startRadius; q <= startRadius; ++q)
    {
        for (int r = -startRadius; r <= startRadius; ++r)
        {
            if ((std::abs(q) + std::abs(q + r) + std::abs(r)) / 2 <= startRadius)
            {
                getTileAt(q, r).discovered = true;
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

World::Tile &Map::getTileAt(int x, int y)
{
    int cx = static_cast<int>(std::floor(static_cast<float>(x) / Config::World::CHUNK_SIZE));
    int cy = static_cast<int>(std::floor(static_cast<float>(y) / Config::World::CHUNK_SIZE));

    uint64_t key = getChunkKey(cx, cy);

    if (m_chunks.find(key) == m_chunks.end())
    {
        World::Chunk *newChunk = new World::Chunk();
        newChunk->x = cx;
        newChunk->y = cy;
        generateChunk(newChunk);
        m_chunks[key] = newChunk;
    }

    int tx = x % Config::World::CHUNK_SIZE;
    int ty = y % Config::World::CHUNK_SIZE;
    if (tx < 0)
        tx += Config::World::CHUNK_SIZE;
    if (ty < 0)
        ty += Config::World::CHUNK_SIZE;

    return m_chunks[key]->tiles[tx][ty];
}

bool Map::isAreaWalkable(int q, int r, int w, int h)
{
    for (int i = q; i < q + w; ++i)
    {
        for (int j = r; j < r + h; ++j)
        {
            World::TileType type = getTileAt(i, j).type;

            if (type == World::TileType::Water || type == World::TileType::Mountain)
            {
                return false;
            }
        }
    }
    return true;
}

void Map::revealRadiusWithCleanup(int centerQ, int centerR, int radius)
{
    int cleanupRadius = radius + 2;

    for (int q = centerQ - cleanupRadius; q <= centerQ + cleanupRadius; ++q)
    {
        for (int r = centerR - cleanupRadius; r <= centerR + cleanupRadius; ++r)
        {
            int dist = (std::abs(centerQ - q) +
                        std::abs(centerQ + centerR - q - r) +
                        std::abs(centerR - r)) /
                       2;

            if (dist <= radius)
            {
                getTileAt(q, r).visible = true;
            }
            else if (dist <= cleanupRadius)
            {
                if (hasTileAt(q, r))
                {
                    getTileAt(q, r).visible = false;
                }
            }
        }
    }
}

void Map::clearAllVisible()
{
    for (auto &pair : m_chunks)
    {
        if (pair.second)
        {
            for (int x = 0; x < Config::World::CHUNK_SIZE; ++x)
            {
                for (int y = 0; y < Config::World::CHUNK_SIZE; ++y)
                {
                    pair.second->tiles[x][y].visible = false;
                }
            }
        }
    }
}

bool Map::hasTileAt(int q, int r)
{
    int cx = static_cast<int>(std::floor(static_cast<float>(q) / Config::World::CHUNK_SIZE));
    int cy = static_cast<int>(std::floor(static_cast<float>(r) / Config::World::CHUNK_SIZE));

    uint64_t key = getChunkKey(cx, cy);

    return (m_chunks.find(key) != m_chunks.end());
}

void Map::debugRevealAll()
{
    for (auto &pair : m_chunks)
    {
        for (int x = 0; x < World::Chunk::Size; ++x)
        {
            for (int y = 0; y < World::Chunk::Size; ++y)
            {
                pair.second->tiles[x][y].visible = true;
            }
        }
    }
}

uint64_t Map::getChunkKey(int cx, int cy) const
{
    uint32_t low = static_cast<uint32_t>(cx);
    uint32_t high = static_cast<uint32_t>(cy);
    return (static_cast<uint64_t>(high) << 32) | low;
}

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

void Map::generateChunk(World::Chunk *chunk)
{
    for (int ty = 0; ty < Config::World::CHUNK_SIZE; ++ty)
    {
        for (int tx = 0; tx < Config::World::CHUNK_SIZE; ++tx)
        {
            int q = chunk->x * Config::World::CHUNK_SIZE + tx;
            int r = chunk->y * Config::World::CHUNK_SIZE + ty;

            float s = Config::World::NOISE_SCALE;
            float e = 1.0f * getNoise(q / s, r / s, m_seed);
            e += 0.5f * getNoise(q / (s / 0.5f), r / (s / 0.5f), m_seed);
            e += 0.25f * getNoise(q / (s / 0.25f), r / (s / 0.25f), m_seed);
            e /= 1.75f;

            float height = e;

            switch (m_mapType)
            {
            case World::MapType::ISLAND:
            {
                float dist = std::sqrt(q * q + r * r) / Config::World::ISLAND_SIZE;
                height = (e + 0.1f) - (dist * dist);
                break;
            }
            case World::MapType::ARCHIPELAGO:
            {
                height = e - 0.2f;
                break;
            }
            case World::MapType::CONTINENTS:
            {
                float cluster = getNoise(q / 500.0f, r / 500.0f, m_seed + 1);
                height = e + (cluster * 0.4f) - 0.2f;
                break;
            }
            case World::MapType::PANGEA:
            {
                height = e + 0.2f;
                break;
            }
            }

            World::Tile &tile = chunk->tiles[tx][ty];
            if (height < Config::World::THRESH_WATER)
                tile.type = World::TileType::Water;
            else if (height < Config::World::THRESH_DIRT)
                tile.type = World::TileType::Dirt;
            else if (height < Config::World::THRESH_GRASS)
                tile.type = World::TileType::Grass;
            else
                tile.type = World::TileType::Mountain;
        }
    }
}
