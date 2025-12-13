#include "DefaultLocationBuilder.h"

DefaultLocationBuilder::DefaultLocationBuilder()
{
    LOG_INFO("Creating DefaultLocationBuilder instance.");
    reset();
}

void DefaultLocationBuilder::reset()
{
    m_location = std::make_unique<Location>();
}

void DefaultLocationBuilder::buildName(Side side)
{
    std::string sideName;
    switch (side)
    {
    case Side::North:
        sideName = "Northern Sector";
        break;
    case Side::South:
        sideName = "Southern Sector";
        break;
    case Side::East:
        sideName = "Eastern Outpost";
        break;
    case Side::West:
        sideName = "Western Foothills";
        break;
    case Side::Random:
    default:
        sideName = "Unknown Area";
        break;
    }
    m_location->setName("Map " + std::to_string(rand() % 100) + " - " + sideName);
}

void DefaultLocationBuilder::buildTiles()
{
    LOG_INFO("Creating default tiles layout.");
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    constexpr int MIN_SIZE = 10;
    constexpr int MAX_SIZE = 25;
    int height = std::rand() % (MAX_SIZE - MIN_SIZE + 1) + MIN_SIZE;
    int width = std::rand() % (MAX_SIZE - MIN_SIZE + 1) + MIN_SIZE;

    std::vector<std::vector<Tile *>> tiles;

    for (int y = 0; y < height; ++y)
    {
        std::vector<Tile *> row;
        for (int x = 0; x < width; ++x)
        {
            if (x == 0 || y == 0 || x == width - 1 || y == height - 1)
            {
                row.push_back(new Tile(ROCK, "#", false));
            }
            else
            {
                row.push_back(new Tile(GRASS, " ", true));
            }
        }
        tiles.push_back(row);
    }
    m_location->setTiles(tiles);
}

void DefaultLocationBuilder::buildResources(Side doorSide)
{
    int height = m_location->getTiles().size();
    int width = (height > 0) ? m_location->getTiles()[0].size() : 0;

    if (width == 0 || height == 0)
        return;

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int numResources = (width * height) / 50;

    std::vector<std::pair<int, int>> availablePositions;
    for (int y = 1; y < height - 1; ++y)
    {
        for (int x = 1; x < width - 1; ++x)
        {
            if (m_location->getTiles()[y][x]->isTraversable())
            {
                availablePositions.push_back({x, y});
            }
        }
    }

    std::shuffle(availablePositions.begin(), availablePositions.end(), std::mt19937(std::random_device()()));

    for (int i = 0; i < numResources && !availablePositions.empty(); ++i)
    {
        auto pos = availablePositions.back();
        availablePositions.pop_back();

        m_location->setTile(pos.first, pos.second, new Tile(SAND, "T", false));
    }

    int doorsToCreate = rand() % 3 + 1;

    auto placeDoor = [&](Side side)
    {
        if (side == Side::Random)
            return;

        int xPos = -1, yPos = -1;

        switch (side)
        {
        case Side::North:
            xPos = rand() % (width - 2) + 1;
            yPos = 0;
            break;
        case Side::South:
            xPos = rand() % (width - 2) + 1;
            yPos = height - 1;
            break;
        case Side::West:
            xPos = 0;
            yPos = rand() % (height - 2) + 1;
            break;
        case Side::East:
            xPos = width - 1;
            yPos = rand() % (height - 2) + 1;
            break;
        default:
            break;
        }

        if (xPos != -1 && yPos != -1)
        {
            m_location->setTile(xPos, yPos, new Tile(GRASS, " ", true));
            LOG_INFO("Placed door at (" + std::to_string(xPos) + ", " + std::to_string(yPos) + ") on side " + std::to_string((int)side));
        }
    };

    placeDoor(doorSide);

    std::vector<Side> allSides = {Side::North, Side::South, Side::East, Side::West};
    std::set<Side> placedDoors = {doorSide};

    for (int i = 0; i < doorsToCreate; ++i)
    {
        Side randomSide = allSides[rand() % allSides.size()];
        if (placedDoors.find(randomSide) == placedDoors.end())
        {
            placeDoor(randomSide);
            placedDoors.insert(randomSide);
        }
    }
}

Location *DefaultLocationBuilder::getLocation()
{
    return m_location.release();
}
