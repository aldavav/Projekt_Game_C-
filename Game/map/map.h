#ifndef MAP_H
#define MAP_H

#include "DefaultLocationBuilder.h"
#include "LocationDirector.h"
#include <LoggerMacros.h>
#include <fstream>
#include <string>
#include <vector>

enum Difficulty
{
    EASY,
    MEDIUM,
    HARD
};

class Map
{
private:
    std::vector<Location *> m_locations;

    Location *m_currentLocation = nullptr;

    Map();

    std::string m_mapName;

    static Map *instance;

    LocationDirector m_locationDirector;

    Difficulty m_difficulty = EASY;

public:
    ~Map();

    static Map &getInstance();

    static void destroyInstance();

    std::string getMapName();

    void initializeNewMap(const std::string &name, Difficulty difficulty);

    Map(const Map &) = delete;

    Map &operator=(const Map &) = delete;

    Map(Map &&) = delete;

    Map &operator=(Map &&) = delete;

    Location *getCurrentLocation();

    void displayCurrentMap();

    void setCurrentLocation(Location *newLocation);

    void switchLocation(size_t locationIndex);

    Location *createNewLocation(bool first = false);

    void saveMap(std::ofstream &file);

    static void loadMap(std::ifstream &file);
};

#endif
