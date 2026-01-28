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

    float minimapVisibleWorldSize = tileS * 1.5f * viewRadius * 2.0f;
    float unitsPerPixel = minimapVisibleWorldSize / (float)mmRect.width();

    float dx = (clickPos.x() - mmRect.width() / 2.0f) * unitsPerPixel;
    float dy = (clickPos.y() - mmRect.height() / 2.0f) * unitsPerPixel;

    return QPointF(cam.getCurrentPos().x() + dx, cam.getCurrentPos().y() + dy);
}

void MinimapProvider::updateCache(int size, int screenWidth, int screenHeight, bool override)
{
    if (m_cache.size() != QSize(size, size))
    {
        m_cache = QPixmap(size, size);
    }

    m_cache.fill(QColor(10, 10, 10, 200));
    QPainter painter(&m_cache);
    painter.setRenderHint(QPainter::Antialiasing);

    auto &map = Map::getInstance();
    auto &cam = Camera::getInstance();

    const float tileS = Config::World::BASE_TILE_SIZE;
    QPointF camPos = cam.getCurrentPos();
    const float radarWorldRadius = Config::World::MINIMAP_VIEW_RADIUS * tileS;

    float dotBaseSize = (tileS / radarWorldRadius) * (size / 2.0f) * 1.5f;

    float finalDotSize = dotBaseSize * 1.5f;

    QPoint centerHex = cam.screenToHex(QPoint(screenWidth / 2, screenHeight / 2), false);

    int scanRange = Config::World::SCAN_RANGE;

    for (int q = centerHex.x() - scanRange; q <= centerHex.x() + scanRange; ++q)
    {
        for (int r = centerHex.y() - scanRange; r <= centerHex.y() + scanRange; ++r)
        {
            int dq = q - centerHex.x();
            int dr = r - centerHex.y();
            int hexDist = (std::abs(dq) + std::abs(dq + dr) + std::abs(dr)) / 2;

            if (hexDist > 40)
                continue;

            if (!map.hasTileAt(q, r) && !override)
                continue;

            World::Tile &tile = map.getTileAt(q, r);
            QColor dotColor = getTileVisualColor(tile, override);

            float worldX = tileS * (1.5f * q);
            float worldY = tileS * (std::sqrt(3.0f) / 2.0f * q + std::sqrt(3.0f) * r);

            float mmX = (size / 2.0f) + ((worldX - camPos.x()) / radarWorldRadius) * (size / 2.0f);
            float mmY = (size / 2.0f) + ((worldY - camPos.y()) / radarWorldRadius) * (size / 2.0f);

            if (mmX >= -dotBaseSize && mmX < size && mmY >= -dotBaseSize && mmY < size)
            {

                float offset = (dotBaseSize - finalDotSize) / 2.0f;
                painter.fillRect(QRectF(mmX + offset, mmY + offset, finalDotSize, finalDotSize), dotColor);
            }
        }
    }

    float zoom = cam.getZoom();
    float viewW = (screenWidth / zoom) / radarWorldRadius * (size / 2.0f);
    float viewH = (screenHeight / zoom) / radarWorldRadius * (size / 2.0f);

    painter.setPen(QPen(Qt::white, 1));
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
