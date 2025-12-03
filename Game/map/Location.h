#ifndef LOCATION_H
#define LOCATION_H

#include <LoggerMacros.h>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <QtGlobal>
#include <fstream>
#include <sstream>
#include <limits>
#include <vector>
#include <string>
#include "Tile.h"

class Player;

enum Side
{
    Random,
    North,
    South,
    East,
    West
};

class Location
{
private:
    std::string m_name;

    std::vector<std::vector<Tile *>> m_tiles;

    std::unordered_map<std::string, size_t> m_destinations;

    Player *m_player = nullptr;

public:
    Location();

    ~Location();

    void setName(const std::string &name);

    void setTiles(const std::vector<std::vector<Tile *>> &tiles);

    const std::vector<std::vector<Tile *>> &getTiles() const;

    std::string getName() const;

    void addDestination(const std::string &coords, size_t mapIndex);

    size_t getDestinationIndex(const std::string &coords) const;

    void setTile(int x, int y, Tile *newTile);

    std::pair<int, int> getOppositeSidePosition(int currentX, int currentY, Location *targetLocation) const;

    Side getSideFromPosition(int x, int y) const;

    Player *getPlayer() const;

    void saveLocation(std::ofstream &file) const;

    static Location *loadLocation(std::ifstream &file);
};

#endif
