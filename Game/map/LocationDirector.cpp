#include "LocationDirector.h"

LocationDirector::LocationDirector() : m_builder(nullptr)
{
    LOG_INFO("Creating LocationDirector instance with no initial builder.");
}

LocationDirector::LocationDirector(LocationBuilder *builder) : m_builder(builder)
{
    LOG_INFO("Creating LocationDirector instance.");
}

LocationDirector::~LocationDirector()
{
    LOG_INFO("Destroying LocationDirector instance.");
    if (m_builder)
    {
        delete m_builder;
        m_builder = nullptr;
    }
}

void LocationDirector::setBuilder(LocationBuilder *builder)
{
    if (m_builder)
    {
        delete m_builder;
    }
    m_builder = builder;
}

Location *LocationDirector::buildLocation(Side doorSide, bool firstLocation)
{
    if (!m_builder)
    {
        LOG_ERROR("LocationBuilder is not set in LocationDirector.");
        return nullptr;
    }

    m_builder->reset();
    m_builder->buildName(doorSide);
    m_builder->buildTiles();
    m_builder->buildResources(doorSide);

    Location *result = m_builder->getLocation();

    return result;
}
