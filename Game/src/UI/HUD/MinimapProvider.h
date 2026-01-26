#ifndef MINIMAPPROVIDER_H
#define MINIMAPPROVIDER_H

#include <Core/Config/Configuration.h>
#include <Core/Common/WorldData.h>
#include <Game/Camera/Camera.h>
#include <Game/Map/Map.h>
#include <QElapsedTimer>
#include <QPainter>
#include <QPixmap>
#include <QPointF>
#include <cmath>

class MinimapProvider
{
public:
    MinimapProvider();

    QPixmap getMinimap(int size, int screenWidth, int screenHeight, bool override = false);

    QPointF screenToWorld(const QPoint &clickPos, const QRect &mmRect);

    void forceUpdate() { m_needsUpdate = true; }

private:
    void updateCache(int size, int screenWidth, int screenHeight, bool override = false);

    QColor getTileVisualColor(const World::Tile &tile, bool override = false);

    QPixmap m_cache;

    QElapsedTimer m_throttleTimer;

    bool m_needsUpdate = true;

    bool m_lastOverride = false;
};

#endif
