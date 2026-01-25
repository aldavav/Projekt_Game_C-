#include "Map.h"

Map &Map::getInstance()
{
    static Map instance;
    return instance;
}

void Map::initializeNewMap(const std::string &name, Engine::Difficulty difficulty)
{
    clear();
    m_mapName = name;
    m_difficulty = difficulty;
    std::random_device rd;
    m_seed = rd();

    int radius = 6;
    int q = 0;
    int r = 0;
    for (int i = q - radius; i <= q + radius; ++i)
    {
        for (int j = r - radius; j <= r + radius; ++j)
        {
            int dist = (std::abs(q - i) + 
                        std::abs(q + r - i - j) + 
                        std::abs(r - j)) / 2;

            if (dist <= radius)
            {
                World::Tile &tile = getTileAt(i, j);
                tile.discovered = true;
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

void Map::clearAllDiscovered()
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

            float dist = (std::abs(q) + std::abs(q + r) + std::abs(r)) / 2.0f;
            float d = dist / Config::World::CHUNK_SIZE;
            float height = (e + Config::World::HEIGHT_BIAS) - (d * d);

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
