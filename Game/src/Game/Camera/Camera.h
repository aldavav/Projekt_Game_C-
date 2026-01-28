#ifndef CAMERA_H
#define CAMERA_H

#include <Core/Config/Configuration.h>
#include <algorithm>
#include <QPointF>
#include <QPoint>
#include <QRect>

class Camera
{
public:
    static Camera &getInstance();

    void update(float deltaTime);

    QPointF screenToWorld(const QPoint &screenPos, bool is3D = Config::World::DEFAULT_3D_VIEW) const;

    QPoint screenToHex(const QPoint &screenPos, bool is3D = Config::World::DEFAULT_3D_VIEW) const;

    QPointF worldToHex(const QPointF &worldPos) const;

    QPoint toScreen(int q, int r, int tileSize = Config::World::BASE_TILE_SIZE, bool is3D = Config::World::DEFAULT_3D_VIEW) const;

    QPointF hexRound(float q, float r) const;

    void move(float dx, float dy);

    void adjustZoom(float delta);

    void shake(float intensity) { m_shakeIntensity = intensity; }

    void setTargetPos(QPointF hexCoords);

    void setViewportSize(int w, int h);

    QPointF getCurrentPos() const { return m_currentPos; }

    float getZoom() const { return m_zoom; }

    int getViewportWidth() const { return static_cast<int>(m_viewportWidth); }

    void setZoom(float newZoom);

    Camera(const Camera &) = delete;

    Camera &operator=(const Camera &) = delete;

    Camera(Camera &&other) noexcept = delete;

    Camera &operator=(Camera &&other) noexcept = delete;

private:
    Camera() = default;

    QPointF m_currentPos;

    QPointF m_targetPos;

    float m_zoom = Config::Gameplay::INITIAL_ZOOM;

    float m_viewportWidth = Config::Gameplay::DEFAULT_WINDOW_WIDTH;

    float m_viewportHeight = Config::Gameplay::DEFAULT_WINDOW_HEIGHT;

    float m_shakeIntensity = 0.0f;

    QPointF m_shakeOffset;
};

#endif
