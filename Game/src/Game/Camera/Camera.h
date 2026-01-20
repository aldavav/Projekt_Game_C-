#ifndef CAMERA_H
#define CAMERA_H

#include <Core/Config/GameConfig.h>
#include <Core/Config/Config.h>
#include <algorithm>
#include <QPointF>
#include <QPoint>
#include <QRect>

class Camera
{
public:
    static Camera &getInstance();

    void update(float deltaTime);

    void handleEdgePanning(const QPoint &mousePos, int viewWidth, int viewHeight, float deltaTime);

    QPointF screenToWorld(const QPoint &screenPos, bool is3D = Config::DEFAULT_3D_VIEW) const;

    QPoint screenToHex(const QPoint &screenPos, bool is3D = Config::DEFAULT_3D_VIEW) const;

    QPoint toScreen(int q, int r, int tileSize = GameConfig::World::BASE_TILE_SIZE, bool is3D = Config::DEFAULT_3D_VIEW) const;

    QPointF hexRound(float q, float r) const;

    void move(float dx, float dy);

    void setTargetPos(QPointF hexCoords);

    void setTargetRawPos(QPointF worldPos);

    void adjustZoom(float delta);

    void shake(float intensity) { m_shakeIntensity = intensity; }

    void setViewportSize(int w, int h);

    void setWorldBounds(const QRect &bounds) { m_worldBounds = bounds; }

    QPointF pos() const { return m_currentPos; }

    float getZoom() const { return m_zoom; }

    int getViewportWidth() const { return m_viewportWidth; }

    int getViewportHeight() const { return m_viewportHeight; }

private:
    Camera() = default;

    ~Camera() = default;

    Camera(const Camera &) = delete;

    Camera &operator=(const Camera &) = delete;

    QPointF m_currentPos;

    QPointF m_targetPos;

    QRect m_worldBounds;

    float m_zoom = Config::INITIAL_ZOOM;

    float m_viewportWidth = Config::DEFAULT_WINDOW_WIDTH;

    float m_viewportHeight = Config::DEFAULT_WINDOW_HEIGHT;

    float m_shakeIntensity = 0.0f;

    QPointF m_shakeOffset;
};

#endif
