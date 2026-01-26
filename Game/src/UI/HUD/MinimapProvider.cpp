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

    float relX = (clickPos.x() - (mmRect.x() + mmRect.width() / 2.0f)) / (float)mmRect.width();
    float relY = (clickPos.y() - (mmRect.y() + mmRect.height() / 2.0f)) / (float)mmRect.height();

    float viewPortScale = cam.getViewportWidth() * Config::World::SCANLINE_SPACING;
    QPointF currentCamPos = cam.getCurrentPos();

    float targetWorldX = currentCamPos.x() + (relX * viewPortScale);
    float targetWorldY = currentCamPos.y() + (relY * viewPortScale);

    return QPointF(targetWorldX, targetWorldY);
}

void MinimapProvider::updateCache(int size, int screenWidth, int screenHeight, bool override)
{
    if (m_cache.size() != QSize(size, size))
    {
        m_cache = QPixmap(size, size);
    }

    m_cache.fill(QColor(10, 10, 10, 200));
    QPainter painter(&m_cache);

    auto &map = Map::getInstance();
    auto &cam = Camera::getInstance();

    const int WORLD_BOUNDS = Config::World::WORLD_BOUNDS_INT;
    const float tileS = Config::World::BASE_TILE_SIZE;
    QPointF camPos = cam.getCurrentPos();
    int viewPortWidth = cam.getViewportWidth() * Config::World::SCANLINE_SPACING;

    for (int q = -WORLD_BOUNDS * 2; q <= WORLD_BOUNDS * 2; ++q)
    {
        int rStart = -WORLD_BOUNDS - (q / 2) - 10;
        int rEnd = WORLD_BOUNDS - (q / 2) + 10;

        for (int r = rStart; r <= rEnd; ++r)
        {
            World::Tile &tile = map.getTileAt(q, r);

            QColor dotColor = getTileVisualColor(tile, override);

            float worldX = tileS * (1.5f * q);
            float worldY = tileS * (0.866f * q + 1.732f * r);

            float mmX = (size / 2.0f) + ((worldX - camPos.x()) / viewPortWidth) * size;
            float mmY = (size / 2.0f) + ((worldY - camPos.y()) / viewPortWidth) * size;

            if (mmX >= 0 && mmX < size && mmY >= 0 && mmY < size)
            {
                painter.fillRect(QRectF(mmX, mmY, 2, 2), dotColor);
            }
        }
    }

    float zoom = cam.getZoom();
    float viewW = (screenWidth / zoom) / viewPortWidth * size;
    float viewH = (screenHeight / zoom) / viewPortWidth * size;
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
