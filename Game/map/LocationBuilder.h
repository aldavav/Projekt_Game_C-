#ifndef LOCATIONBUILDER_H
#define LOCATIONBUILDER_H

#include "Location.h"
#include <memory>

class LocationBuilder
{
public:
    virtual ~LocationBuilder() = default;

    virtual void buildName(Side side) = 0;

    virtual void buildTiles() = 0;

    virtual void buildResources(Side side) = 0;

    virtual Location *getLocation() = 0;

    virtual void reset() = 0;
};

#endif
