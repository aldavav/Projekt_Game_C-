#include "Map.h"

Map &Map::getInstance()
{
    static Map instance;
    return instance;
}

void Map::initializeNewMap(const std::string &name, Difficulty difficulty)
{
    clear();
    m_mapName = name;
    m_difficulty = difficulty;
    std::random_device rd;
    m_seed = rd();
}

void Map::clear()
{
    for (auto &pair : m_chunks)
    {
        delete pair.second;
    }
    m_chunks.clear();
}

Tile &Map::getTileAt(int x, int y)
{
    int cx = static_cast<int>(std::floor(static_cast<float>(x) / GameConfig::CHUNK_SIZE));
    int cy = static_cast<int>(std::floor(static_cast<float>(y) / GameConfig::CHUNK_SIZE));

    uint64_t key = getChunkKey(cx, cy);

    if (m_chunks.find(key) == m_chunks.end())
    {
        Chunk *newChunk = new Chunk();
        newChunk->x = cx;
        newChunk->y = cy;
        generateChunk(newChunk);
        m_chunks[key] = newChunk;
    }

    int tx = x % GameConfig::CHUNK_SIZE;
    int ty = y % GameConfig::CHUNK_SIZE;
    if (tx < 0)
        tx += GameConfig::CHUNK_SIZE;
    if (ty < 0)
        ty += GameConfig::CHUNK_SIZE;

    return m_chunks[key]->tiles[tx][ty];
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

void Map::revealRadius(int centerQ, int centerR, int radius)
{
    for (int q = centerQ - radius; q <= centerQ + radius; ++q)
    {
        for (int r = centerR - radius; r <= centerR + radius; ++r)
        {
            int dist = (std::abs(centerQ - q) + std::abs(centerQ + centerR - q - r) + std::abs(centerR - r)) / 2;
            if (dist <= radius)
            {
                getTileAt(q, r).discovered = true;
            }
        }
    }
}

void Map::debugRevealAll()
{
    for (auto &pair : m_chunks)
    {
        for (int x = 0; x < Chunk::SIZE; ++x)
        {
            for (int y = 0; y < Chunk::SIZE; ++y)
            {
                pair.second->tiles[x][y].discovered = true;
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

void Map::generateChunk(Chunk *chunk)
{
    for (int ty = 0; ty < GameConfig::CHUNK_SIZE; ++ty)
    {
        for (int tx = 0; tx < GameConfig::CHUNK_SIZE; ++tx)
        {
            int q = chunk->x * GameConfig::CHUNK_SIZE + tx;
            int r = chunk->y * GameConfig::CHUNK_SIZE + ty;

            float s = GameConfig::NOISE_SCALE;
            float e = 1.0f * getNoise(q / s, r / s, m_seed);
            e += 0.5f * getNoise(q / (s / 0.5f), r / (s / 0.5f), m_seed);
            e += 0.25f * getNoise(q / (s / 0.25f), r / (s / 0.25f), m_seed);
            e /= 1.75f;

            float dist = (std::abs(q) + std::abs(q + r) + std::abs(r)) / 2.0f;
            float d = dist / GameConfig::MAP_GENERATION_RADIUS;
            float height = (e + GameConfig::MAP_HEIGHT_BIAS) - (d * d);

            Tile &tile = chunk->tiles[tx][ty];
            if (height < GameConfig::THRESHOLD_WATER)
                tile.type = TileType::WATER;
            else if (height < GameConfig::THRESHOLD_DIRT)
                tile.type = TileType::DIRT;
            else if (height < GameConfig::THRESHOLD_GRASS)
                tile.type = TileType::GRASS;
            else
                tile.type = TileType::MOUNTAIN;

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
            case TileType::DIRT:
                m_stats.dirtCount++;
                break;
            }
        }
    }
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
