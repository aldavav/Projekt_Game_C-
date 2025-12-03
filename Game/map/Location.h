#ifndef NEW_PROJEKT_ZOO_LOCATION_H
#define NEW_PROJEKT_ZOO_LOCATION_H

#include <optional>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <memory> // Include for std::unique_ptr
#include "Tile.h"
#include "../player/Player.h"

enum Side {
    Top,
    Right,
    Bottom,
    Left,
    Random
};

class Location {
private:
    std::vector<std::vector<Tile*>> m_tiles;

    std::unordered_map<std::string, size_t> m_destinations;

    int m_width;

    int m_height;

    // FIX: Using std::unique_ptr to manage the allocated Tile, preventing the memory leak
    // and ensuring it's cleaned up when Location is destroyed.
    std::unique_ptr<Tile> m_previousTile = std::make_unique<Tile>(' ', true, nullptr);

    std::string m_name;

    Location(const std::string& name, const std::vector<std::vector<Tile*>>& tiles, const std::unordered_map<std::string, size_t>& destinations, int width, int height, Tile* tile);

public:
    // Default constructor implementation now uses the unique_ptr initializer
    Location(std::string name) : m_name(std::move(name)) {}

    // Destructor is now default as unique_ptr handles cleanup.
    ~Location() = default;

    std::vector<std::vector<Tile*>> getTiles();

    Player* getPlayer();

    void setTile(int x, int y, Tile* tile);

    bool move(int currentX, int currentY, int newX, int newY, Tile* newTile);

    void addDestination(const std::string& destinationId, size_t locationIndex);

    size_t getDestinationIndex(const std::string& destinationId);

    std::pair<int, int> getOppositeSidePosition(int x, int y, const Location* newLocation);

    Side getSideFromPosition(int x, int y);

    std::string getName();

    bool setPreviousTile(Tile* tile);

    void setHeight(int height);

    void setWidth(int width);

    void setTile(std::vector<std::vector<Tile*>> tiles);

    void saveLocation(std::ofstream &file);

    static Location* loadLocation(std::ifstream &file);
};
#endif //NEW_PROJEKT_ZOO_LOCATION_H
