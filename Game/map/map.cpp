//
// Created by Michael Szotkowski on 6/17/2024.
//

/*#include "Map.h"
#include <algorithm> // Required for std::all_of

Map* Map::instance = nullptr;

// Provide concrete definitions for missing constructors to fix linker errors.

// DefaultLocationBuilder default ctor (was declared but not defined).
DefaultLocationBuilder::DefaultLocationBuilder() { }

// LocationDirector ctor accepting a LocationBuilder* (was declared but not defined).
LocationDirector::LocationDirector(LocationBuilder* builder) : m_builder(nullptr) {
    if (builder) {
        m_builder = builder;
    } else {
        m_builder = new DefaultLocationBuilder();
    }
}

// NEW: Combined initialization function
void Map::initializeNewMap(const std::string& name, Difficulty difficulty) {
    // 1. Set Name and Difficulty
    m_mapName = name;
    m_difficulty = difficulty;
    // LOG_INFO("Map initialized: Name=" + name + ", Difficulty=" + std::to_string(m_difficulty));

    // 2. Set the Director's Builder based on difficulty
    /*
    switch (m_difficulty) {
        case EASY:
            m_locationDirector.setBuilder(new EasyLocationBuilder());
            break;
        case MEDIUM:
            m_locationDirector.setBuilder(new MediumLocationBuilder());
            break;
        case HARD:
            m_locationDirector.setBuilder(new HardLocationBuilder());
            break;
    }
    */

    // 3. Create the first location and set it as current
    /*createNewLocation(true);

    // Optional: Place the player in the new location here, if not handled in createNewLocation(true)
}

std::string Map::getMapName() {
    return m_mapName;
}

Location* Map::getCurrentLocation() {
    return m_currentLocation;
}

Map& Map::getInstance() {
    /*if (instance == nullptr) {
        instance = new Map();
    }
    return *instance;*/
/*}

void Map::destroyInstance() {
    //delete instance;
    //instance = nullptr;
}

void Map::displayCurrentMap() {
    if (m_currentLocation) {
        /*std::cout << "Current Location: " << m_currentLocation->getName() << std::endl;
        for (const auto& row : m_currentLocation->getTiles()) {
            for (const auto& tile : row) {
                std::cout << tile->getSymbol() << " ";
            }
            std::cout << std::endl;
        }*/
    /*} else {
        //LOG_ERROR("No current location set.");
        //std::cerr << RED_TEXT << "No current location set." << RESET_TEXT << std::endl;
    }
}

void Map::setCurrentLocation(Location* location) {
    m_currentLocation = location;
}

void Map::switchLocation(size_t locationIndex) {
    //Player* player = getInstance().getCurrentLocation()->getPlayer();
    Location* location = m_locations[locationIndex];
    /*std::pair<int, int> oppositeSidePosition = m_currentLocation->getOppositeSidePosition(player->getX(), player->getY(), location);
    m_currentLocation->setTile(player->getX(), player->getY(), new Tile(' ', true));
    player->setXY(oppositeSidePosition);
    location->setTile(player->getX(), player->getY(), new Tile(player->getPlayerSymbol(), false, player));*/
    /*m_currentLocation = location;
}

Location* Map::createNewLocation(bool first) {
    //LOG_INFO("Map::createNewLocation: Starting location creation.");

    // --- CASE: Not the first location (Map Change Triggered) ---
    if (!first) {
        // 1. Get Player and Side
        //Player* player = getInstance().getCurrentLocation()->getPlayer();
        /*if (!player) {
            //LOG_ERROR("Map::createNewLocation: Player entity not found in current location (CRITICAL).");
            // Returning nullptr will likely crash the game later, but logs the error clearly.
            return nullptr;
        }*/
        /*Side doorSide = getInstance().getCurrentLocation()->getSideFromPosition(player->getX(), player->getY());
        //LOG_INFO("Map::createNewLocation: Player found. Current position: (" + std::to_string(player->getX()) + ", " + std::to_string(player->getY()) + "). Door Side: " + std::to_string(doorSide));

        // Store old coordinates to clear the tile *later*
        int oldX = player->getX();
        int oldY = player->getY();

        // 2. Create New Location
        Location* newLocation = m_locationDirector.buildLocation(doorSide);
        // Using internal vector size for robust logging
        size_t new_rows = newLocation->getTiles().size();
        size_t new_cols = (new_rows > 0) ? newLocation->getTiles()[0].size() : 0;

        //LOG_INFO("Map::createNewLocation: New location created. Actual Array Dimensions: [Rows (Y)=" + std::to_string(new_rows) + "][Cols (X)=" + std::to_string(new_cols) + "].");
        m_locations.push_back(newLocation);

        // --- PLAYER TRANSFER LOGIC START ---

        // 3. Calculate New Position and Update Player Object
        // This calculates the destination on the *new* map based on the exit side of the *old* map.
        std::pair<int, int> oppositeSidePosition = m_currentLocation->getOppositeSidePosition(oldX, oldY, newLocation);
        int newX = oppositeSidePosition.first;
        int newY = oppositeSidePosition.second;

        //LOG_INFO("Map::createNewLocation: Calculated new player coordinates: (" + std::to_string(newX) + ", " + std::to_string(newY) + ")");

        // --- SAFETY CHECK FOR OUT-OF-BOUNDS ERROR ---
        if (newX < 0 || newX > (int)new_cols || newY < 0 || newY > (int)new_rows) {
            //LOG_ERROR("Map::createNewLocation: CRITICAL BOUNDARY ERROR: Calculated coordinates (" + std::to_string(newX) + ", " + std::to_string(newY) + ") are outside the new map bounds (W:" + std::to_string(new_cols) + ", H:" + std::to_string(new_rows) + "). Player placement aborted.");
            // We can't safely proceed, but we'll try to set the player to (0,0) as a recovery measure.
            // A proper fix requires correcting Location::getOppositeSidePosition.
            // For now, we abort placement but continue the switch to prevent a full crash loop.
            // The GameEngine will catch the missing player on the next loop.
            oppositeSidePosition.first--;
            oppositeSidePosition.second--;
        }
            // Update Player and Tiles ONLY if coordinates are safe
            player->setXY(oppositeSidePosition); // Update player's internal coordinates

            // 4. Update Tiles

            // Clear the player from the OLD location tile
            m_currentLocation->setTile(oldX, oldY, new Tile(' ', true));
            //LOG_INFO("Map::createNewLocation: Cleared player tile from OLD location at (" + std::to_string(oldX) + ", " + std::to_string(oldY) + ")");

            // Set player's tile in the NEW location grid
            newLocation->setTile(newX, newY, new Tile(player->getPlayerSymbol(), false, player));
            //LOG_INFO("Map::createNewLocation: Placed player onto NEW location at (" + std::to_string(newX) + ", " + std::to_string(newY) + ")");

            // 5. Update Destinations (Portals)

            // Add destination from old location to new location
            m_currentLocation->addDestination(std::to_string(oldX) + "," + std::to_string(oldY), m_locations.size() - 1);
            //LOG_INFO("Map::createNewLocation: Destination added: OLD->NEW, Index: " + std::to_string(m_locations.size() - 1));

            // Add destination from new location back to old location
            int oldLocationIndex = std::find(m_locations.begin(), m_locations.end(), m_currentLocation) - m_locations.begin();
            newLocation->addDestination(std::to_string(newX) + "," + std::to_string(newY), oldLocationIndex);
            //LOG_INFO("Map::createNewLocation: Destination added: NEW->OLD, Index: " + std::to_string(oldLocationIndex));


        // 6. Update Current Location Pointer
        m_currentLocation = newLocation;
        //LOG_INFO("Map::createNewLocation: Location switch complete. m_currentLocation pointer updated.");
    
        return newLocation;*/

    // --- CASE: First location (Initial Game Load) ---
    /*} else {
        //LOG_INFO("Map::createNewLocation: Creating FIRST location.");
        // If there are no existing locations, create a new one without specifying the door side
        //Location* newLocation = m_locationDirector.buildLocation(Random, true);
        //m_locations.push_back(newLocation);
        //m_currentLocation = newLocation;
        //return newLocation;
    }
}

void Map::saveMap(std::ofstream &file) {
    //LOG_INFO("Saving Map instance to file.");

    // Save Difficulty first
    file.write(reinterpret_cast<const char*>(&m_difficulty), sizeof(Difficulty));

    // Save Map Name
    size_t nameSize = m_mapName.size();
    file.write(reinterpret_cast<const char*>(&nameSize), sizeof(size_t));
    file.write(m_mapName.c_str(), nameSize);

    // Save Locations
    size_t numLocations = m_locations.size();
    file.write(reinterpret_cast<const char*>(&numLocations), sizeof(size_t));
    for (const auto& loc : m_locations) {
        //loc->saveLocation(file);
    }
}

void Map::loadMap(std::ifstream &file) {
    //LOG_INFO("Creating Map instance from file.");
    /*destroyInstance();

    Map& mapInstance = getInstance();

    // Load Difficulty first
    Difficulty tempDifficulty;
    file.read(reinterpret_cast<char*>(&tempDifficulty), sizeof(Difficulty));

    // Load Map Name
    size_t nameSize;
    file.read(reinterpret_cast<char*>(&nameSize), sizeof(size_t));
    std::string tempMapName(nameSize, '\0');
    file.read(&tempMapName[0], nameSize);

    // Load Locations
    size_t numLocations;
    file.read(reinterpret_cast<char*>(&numLocations), sizeof(size_t));
    std::vector<Location*> tempLocations;
    for (size_t i = 0; i < numLocations; ++i) {
        //Location* loc = Location::loadLocation(file);
        //tempLocations.push_back(loc);
    }

    // Assign to instance
    mapInstance.m_difficulty = tempDifficulty;
    mapInstance.m_mapName = tempMapName;
    mapInstance.m_locations = tempLocations;

    // Optional: Re-set the LocationDirector builder based on loaded difficulty
    /*
    switch (mapInstance.m_difficulty) {
        case EASY:
            mapInstance.m_locationDirector.setBuilder(new EasyLocationBuilder());
            break;
        case MEDIUM:
            mapInstance.m_locationDirector.setBuilder(new MediumLocationBuilder());
            break;
        case HARD:
            mapInstance.m_locationDirector.setBuilder(new HardLocationBuilder());
            break;
    }
    */
/*}*/
