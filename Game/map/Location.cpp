#include "Location.h"

static size_t getLocationWidth(const std::vector<std::vector<Tile *>> &tiles)
{
    return tiles.empty() ? 0 : tiles[0].size();
}

static size_t getLocationHeight(const std::vector<std::vector<Tile *>> &tiles)
{
    return tiles.size();
}

Location::Location()
{
}

Location::~Location()
{
    for (auto &row : m_tiles)
    {
        for (auto &tile : row)
        {
            delete tile;
        }
    }
    m_tiles.clear();
    m_player = nullptr;
}

void Location::setName(const std::string &name)
{
    m_name = name;
}

void Location::setTiles(const std::vector<std::vector<Tile *>> &tiles)
{
    for (auto &row : m_tiles)
    {
        for (auto &tile : row)
        {
            delete tile;
        }
    }
    m_tiles = tiles;
}

const std::vector<std::vector<Tile *>> &Location::getTiles() const
{
    return m_tiles;
}

std::string Location::getName() const
{
    return m_name;
}

void Location::addDestination(const std::string &coords, size_t mapIndex)
{
    m_destinations[coords] = mapIndex;
}

size_t Location::getDestinationIndex(const std::string &destinationId) const
{
    auto it = m_destinations.find(destinationId);
    if (it != m_destinations.end())
    {
        return it->second;
    }
    return std::numeric_limits<size_t>::max();
}

void Location::setTile(int x, int y, Tile *newTile)
{
    size_t rows = getLocationHeight(m_tiles);
    size_t cols = getLocationWidth(m_tiles);

    if (static_cast<size_t>(y) < rows && static_cast<size_t>(x) < cols)
    {
        delete m_tiles[y][x];
        m_tiles[y][x] = newTile;
    }
    else
    {
        LOG_ERROR("Attempted to set tile out of bounds at (" + std::to_string(x) + ", " + std::to_string(y) + ").");
        delete newTile;
    }
}

std::pair<int, int> Location::getOppositeSidePosition(int currentX, int currentY, Location *targetLocation) const
{
    Side playerSide = getSideFromPosition(currentX, currentY);

    size_t targetRows = getLocationHeight(targetLocation->m_tiles);
    size_t targetCols = getLocationWidth(targetLocation->m_tiles);

    int newX = -1;
    int newY = -1;

    switch (playerSide)
    {
    case Side::North:
        newY = targetRows - 1;
        for (size_t col = 0; col < targetCols; ++col)
        {
            if (targetLocation->m_tiles[newY][col]->isTraversable())
            {
                newX = col;
                return std::make_pair(newX, newY);
            }
        }
        break;

    case Side::South:
        newY = 0;
        for (size_t col = 0; col < targetCols; ++col)
        {
            if (targetLocation->m_tiles[newY][col]->isTraversable())
            {
                newX = col;
                return std::make_pair(newX, newY);
            }
        }
        break;

    case Side::East:
        newX = 0;
        for (size_t row = 0; row < targetRows; ++row)
        {
            if (targetLocation->m_tiles[row][newX]->isTraversable())
            {
                newY = row;
                return std::make_pair(newX, newY);
            }
        }
        break;

    case Side::West:
        newX = targetCols - 1;
        for (size_t row = 0; row < targetRows; ++row)
        {
            if (targetLocation->m_tiles[row][newX]->isTraversable())
            {
                newY = row;
                return std::make_pair(newX, newY);
            }
        }
        break;

    case Side::Random:
    default:
        break;
    }

    LOG_WARNING("No passable entry point found for map transition.");
    return std::make_pair(-1, -1);
}

Side Location::getSideFromPosition(int x, int y) const
{
    size_t rows = getLocationHeight(m_tiles);
    size_t cols = getLocationWidth(m_tiles);

    if (y == 0 && rows > 0)
    {
        return Side::North;
    }
    else if (y == static_cast<int>(rows) - 1 && rows > 0)
    {
        return Side::South;
    }
    else if (x == 0 && cols > 0)
    {
        return Side::West;
    }
    else if (x == static_cast<int>(cols) - 1 && cols > 0)
    {
        return Side::East;
    }
    else
    {
        return Side::Random;
    }
}

void Location::setPlayer(Player *player)
{
    if (m_player && m_player != player)
    {
        LOG_WARNING("Overwriting existing player reference in location " + m_name);
    }
    m_player = player;
}

Player *Location::removePlayer()
{
    Player *p = m_player;
    m_player = nullptr;
    return p;
}

Player *Location::getPlayer() const
{
    if (!m_player)
    {
        LOG_INFO("No player/faction explicitly set for this location: " + m_name);
    }
    return m_player;
}

void Location::saveLocation(std::ofstream &file) const
{
    LOG_INFO("Saving location: " + m_name + ".");

    size_t nameSize = m_name.size();
    file.write(reinterpret_cast<const char *>(&nameSize), sizeof(nameSize));
    file.write(m_name.c_str(), nameSize);

    size_t numRows = getLocationHeight(m_tiles);
    file.write(reinterpret_cast<const char *>(&numRows), sizeof(size_t));

    for (const auto &row : m_tiles)
    {
        size_t numTiles = row.size();
        file.write(reinterpret_cast<const char *>(&numTiles), sizeof(size_t));
        for (const auto &tile : row)
        {
            tile->saveTile(file);
        }
    }

    size_t numDestinations = m_destinations.size();
    file.write(reinterpret_cast<const char *>(&numDestinations), sizeof(size_t));
    for (const auto &door : m_destinations)
    {
        size_t destinationIdLength = door.first.size();
        file.write(reinterpret_cast<const char *>(&destinationIdLength), sizeof(size_t));
        file.write(door.first.c_str(), destinationIdLength);
        file.write(reinterpret_cast<const char *>(&door.second), sizeof(size_t));
    }
}

Location *Location::loadLocation(std::ifstream &file)
{
    size_t name_length;
    if (!file.read(reinterpret_cast<char *>(&name_length), sizeof(name_length)))
        return nullptr;
    std::string name(name_length, '\0');
    if (!file.read(&name[0], name_length))
        return nullptr;

    size_t numRows;
    if (!file.read(reinterpret_cast<char *>(&numRows), sizeof(size_t)))
        return nullptr;
    std::vector<std::vector<Tile *>> tiles;
    for (size_t i = 0; i < numRows; ++i)
    {
        size_t numTiles;
        if (!file.read(reinterpret_cast<char *>(&numTiles), sizeof(size_t)))
            return nullptr;
        std::vector<Tile *> row;
        for (size_t j = 0; j < numTiles; ++j)
        {
            Tile *tile = Tile::loadTile(file);
            if (!tile)
                return nullptr;
            row.push_back(tile);
        }
        tiles.push_back(row);
    }

    size_t numDestinations;
    if (!file.read(reinterpret_cast<char *>(&numDestinations), sizeof(size_t)))
        return nullptr;
    std::unordered_map<std::string, size_t> destinations;
    for (size_t i = 0; i < numDestinations; ++i)
    {
        size_t destinationIdLength;
        if (!file.read(reinterpret_cast<char *>(&destinationIdLength), sizeof(size_t)))
            return nullptr;
        std::string destinationId(destinationIdLength, '\0');
        if (!file.read(&destinationId[0], destinationIdLength))
            return nullptr;

        size_t locationIndex;
        if (!file.read(reinterpret_cast<char *>(&locationIndex), sizeof(size_t)))
            return nullptr;
        destinations[destinationId] = locationIndex;
    }

    LOG_INFO("Successfully loaded location: " + name);
    Location *location = new Location();
    location->setName(name);
    location->setTiles(tiles);
    location->m_destinations = destinations;

    return location;
}
