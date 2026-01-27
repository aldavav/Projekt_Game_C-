#include "MinimapProvider.h"

MinimapProvider::MinimapProvider()
{
    m_throttleTimer.start();
}

QPixmap MinimapProvider::getMinimap(int size, int screenWidth, int screenHeight, bool override)
{
    if (override != m_lastOverride || m_needsUpdate || m_throttleTimer.elapsed() > Config::World::MINIMAP_UPDATE_MS)
    {
        m_lastOverride = override;

        updateCache(size, screenWidth, screenHeight, override);

        m_needsUpdate = false;
        m_throttleTimer.restart();
    }
    return m_cache;
}

QPointF MinimapProvider::screenToWorld(const QPoint &clickPos, const QRect &mmRect)
{
    auto &cam = Camera::getInstance();
    const float tileS = Config::World::BASE_TILE_SIZE;
    const float viewRadius = Config::World::MINIMAP_VIEW_RADIUS;

    // The size factor used in updateCache
    float minimapVisibleWorldSize = tileS * 1.5f * viewRadius * 2.0f;
    float unitsPerPixel = minimapVisibleWorldSize / (float)mmRect.width();

    // 1. Calculate how many Cartesian units (pixels-worth) we are from the center
    float dx = (clickPos.x() - mmRect.center().x()) * unitsPerPixel;
    float dy = (clickPos.y() - mmRect.center().y()) * unitsPerPixel;

    // 2. Convert Cartesian Delta (dx, dy) back to Axial Delta (dq, dr)
    // This is the inverse of your worldX/worldY math
    float dq = dx / (tileS * 1.5f);
    float dr = (dy - (tileS * 0.866f * dq)) / (tileS * 1.732f);

    // 3. Add the delta to the current camera axial position
    return QPointF(cam.getCurrentPos().x() + dq,
                   cam.getCurrentPos().y() + dr);
}

void MinimapProvider::updateCache(int size, int screenWidth, int screenHeight, bool override)
{
    if (m_cache.size() != QSize(size, size))
        m_cache = QPixmap(size, size);

    m_cache.fill(Qt::transparent); // Use transparent or a dark background
    QPainter painter(&m_cache);
    painter.setRenderHint(QPainter::Antialiasing);

    auto &map = Map::getInstance();
    auto &cam = Camera::getInstance();

    const int BOUNDS = Config::World::WORLD_BOUNDS_INT;
    const float tileS = Config::World::BASE_TILE_SIZE;
    QPointF camPos = cam.getCurrentPos();

    // Use the MINIMAP_VIEW_RADIUS we discussed earlier for the zoom level
    float viewRadius = Config::World::MINIMAP_VIEW_RADIUS;
    float minimapVisibleWorldSize = tileS * 1.5f * viewRadius * 2.0f;
    float pixelsPerUnit = size / minimapVisibleWorldSize;

    // HEXAGONAL LOOP: Iterate q from -BOUNDS to +BOUNDS
    for (int q = -BOUNDS; q <= BOUNDS; ++q)
    {
        // For a hexagon, r is constrained by both the bounds AND q
        int rStart = std::max(-BOUNDS, -q - BOUNDS);
        int rEnd = std::min(BOUNDS, -q + BOUNDS);

        for (int r = rStart; r <= rEnd; ++r)
        {
            World::Tile &tile = map.getTileAt(q, r);
            QColor dotColor = getTileVisualColor(tile, override);

            // Calculate World Position
            float worldX = tileS * (1.5f * q);
            float worldY = tileS * (0.866f * q + 1.732f * r);

            // Convert to Minimap Pixels (Centered on Camera)
            float mmX = (size / 2.0f) + (worldX - camPos.x()) * pixelsPerUnit;
            float mmY = (size / 2.0f) + (worldY - camPos.y()) * pixelsPerUnit;

            // Only draw if within the minimap pixmap
            if (mmX >= -2 && mmX < size + 2 && mmY >= -2 && mmY < size + 2)
            {
                // We use 2.5 to slightly overlap tiles and avoid "black lines" between dots
                painter.fillRect(QRectF(mmX, mmY, 2.5f, 2.5f), dotColor);
            }
        }
    }

    // DRAW VIEWPORT BOX
    float zoom = cam.getZoom();
    float viewW = (screenWidth / zoom) * pixelsPerUnit;
    float viewH = (screenHeight / zoom) * pixelsPerUnit;
    
    painter.setPen(QPen(QColor(255, 255, 255, 200), 1));
    painter.drawRect(QRectF((size - viewW) / 2.0f, (size - viewH) / 2.0f, viewW, viewH));
}

QColor MinimapProvider::getTileVisualColor(const World::Tile &tile, bool override)
{
    QColor baseColor;

    switch (tile.type)
    {
    case World::TileType::Water:
        baseColor = Config::UI::COLOR_WATER;
        break;
    case World::TileType::Grass:
        baseColor = Config::UI::COLOR_GRASS;
        break;
    case World::TileType::Mountain:
        baseColor = Config::UI::COLOR_MOUNTAIN;
        break;
    case World::TileType::Dirt:
        baseColor = Config::UI::COLOR_DIRT;
        break;
    default:
        baseColor = Config::UI::COLOR_WATER;
        break;
    }

    if (override || tile.discovered)
    {
        return baseColor;
    }
    else
    {
        return baseColor.darker(250);
    }

    return Qt::black;
}
