#include "Map.h"

Map *Map::instance = nullptr;

Map::Map()
{
    m_locationDirector.setBuilder(new DefaultLocationBuilder());
}

Map::~Map()
{
    for (Location *loc : m_locations)
    {
        delete loc;
    }
    m_locations.clear();
}

Map &Map::getInstance()
{
    if (instance == nullptr)
    {
        instance = new Map();
    }
    return *instance;
}

void Map::destroyInstance()
{
    delete instance;
    instance = nullptr;
}

void Map::initializeNewMap(const std::string &name, Difficulty difficulty)
{
    m_mapName = name;
    m_difficulty = difficulty;
    LOG_INFO("Map initialized: Name=" + name + ", Difficulty=" + std::to_string(m_difficulty));
    createNewLocation(true);
}

Location *Map::getCurrentLocation()
{
    return m_currentLocation;
}

std::string Map::getMapName()
{
    return m_mapName;
}

void Map::displayCurrentMap()
{
    if (m_currentLocation)
    {
        LOG_INFO("Current Location: " + m_currentLocation->getName());
    }
    else
    {
        LOG_ERROR("No current location set.");
    }
}

void Map::setCurrentLocation(Location *location)
{
    m_currentLocation = location;
}

void Map::switchLocation(size_t locationIndex)
{
    if (locationIndex >= m_locations.size())
    {
        LOG_ERROR("Invalid location index for switchLocation.");
        return;
    }
    m_currentLocation = m_locations[locationIndex];
    LOG_INFO("Switched to location at index: " + std::to_string(locationIndex));
}

Location *Map::createNewLocation(bool first)
{
    if (!m_locationDirector.buildLocation(Random, true))
    {
        LOG_ERROR("Failed to create location via director.");
        return nullptr;
    }
    Location *newLocation = m_locationDirector.buildLocation(Random, first);
    m_locations.push_back(newLocation);

    if (first)
    {
        m_currentLocation = newLocation;
        LOG_INFO("Created and set first location: " + newLocation->getName());
    }
    else
    {
        Player *player = m_currentLocation->getPlayer();
        if (player)
        {
            /*Side doorSide = m_currentLocation->getSideFromPosition(player->getX(), player->getY());
            std::pair<int, int> oppositeSidePosition = m_currentLocation->getOppositeSidePosition(player->getX(), player->getY(), newLocation);
            int oldX = player->getX();
            int oldY = player->getY();
            int newX = oppositeSidePosition.first;
            int newY = oppositeSidePosition.second;

            player->setXY(oppositeSidePosition);

            m_currentLocation->setTile(oldX, oldY, new Tile(GRASS, " ", true));
            newLocation->setTile(newX, newY, new Tile(GRASS, player->getPlayerSymbol(), false));

            m_currentLocation->addDestination(std::to_string(oldX) + "," + std::to_string(oldY), m_locations.size() - 1);

            int oldLocationIndex = std::find(m_locations.begin(), m_locations.end(), m_currentLocation) - m_locations.begin();
            newLocation->addDestination(std::to_string(newX) + "," + std::to_string(newY), oldLocationIndex);*/

            m_currentLocation = newLocation;
            LOG_INFO("Created and switched to new location: " + newLocation->getName());
        }
        else
        {
            LOG_ERROR("Cannot switch location: Player entity not found in current location.");
        }
    }
    return newLocation;
}

void Map::saveMap(std::ofstream &file)
{
    LOG_INFO("Saving Map instance to file.");
    file.write(reinterpret_cast<const char *>(&m_difficulty), sizeof(Difficulty));
    size_t nameSize = m_mapName.size();
    file.write(reinterpret_cast<const char *>(&nameSize), sizeof(size_t));
    file.write(m_mapName.c_str(), nameSize);
    size_t numLocations = m_locations.size();
    file.write(reinterpret_cast<const char *>(&numLocations), sizeof(size_t));
}

void Map::loadMap(std::ifstream &file)
{
    LOG_INFO("Loading Map instance from file.");
    destroyInstance();
    Map &mapInstance = getInstance();
    Difficulty tempDifficulty;
    file.read(reinterpret_cast<char *>(&tempDifficulty), sizeof(Difficulty));
    size_t nameSize;
    file.read(reinterpret_cast<char *>(&nameSize), sizeof(size_t));
    std::string tempMapName(nameSize, '\0');
    file.read(&tempMapName[0], nameSize);
    size_t numLocations;
    file.read(reinterpret_cast<char *>(&numLocations), sizeof(size_t));
    mapInstance.m_difficulty = tempDifficulty;
    mapInstance.m_mapName = tempMapName;
}
