#ifndef LOCATION_H
#define LOCATION_H

#include "../Logger/LoggerMacros.h"
#include <unordered_map>
#include <fstream>
#include <utility>
#include <vector>
#include <string>
#include <limits>
#include "Tile.h"

class Player;
class Tile;

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

    void setPlayer(Player *player);

    Player *removePlayer();

    Player *getPlayer() const;

    void saveLocation(std::ofstream &file) const;

    static Location *loadLocation(std::ifstream &file);

    void addDestination(const std::string &coords, size_t mapIndex);

    size_t getDestinationIndex(const std::string &destinationId) const;

    std::pair<int, int> getOppositeSidePosition(int currentX, int currentY, Location *targetLocation) const;

    Side getSideFromPosition(int x, int y) const;

    void setTile(int x, int y, Tile *newTile);
};

#endif
