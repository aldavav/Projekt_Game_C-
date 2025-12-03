#ifndef LOCATIONDIRECTOR_H
#define LOCATIONDIRECTOR_H

#include "LocationBuilder.h"
#include <LoggerMacros.h>

class LocationDirector
{
private:
    LocationBuilder *m_builder;

public:
    LocationDirector();

    LocationDirector(LocationBuilder *builder);

    ~LocationDirector();

    void setBuilder(LocationBuilder *builder);

    Location *buildLocation(Side side, bool isFirst = false);
};

#endif
