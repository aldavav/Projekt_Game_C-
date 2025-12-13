#ifndef DEFAULTLOCATIONBUILDER_H
#define DEFAULTLOCATIONBUILDER_H

#include "LocationBuilder.h"
#include <LoggerMacros.h>
#include "Location.h"
#include <algorithm>
#include <cstdlib>
#include <random>
#include <memory>
#include <ctime>
#include <set>

class DefaultLocationBuilder : public LocationBuilder
{
private:
    std::unique_ptr<Location> m_location;

public:
    DefaultLocationBuilder();

    ~DefaultLocationBuilder() = default;

    void buildName(Side side) override;

    void buildTiles() override;

    void buildResources(Side side) override;

    Location *getLocation() override;

    void reset() override;
};

#endif
