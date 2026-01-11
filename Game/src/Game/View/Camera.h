#ifndef CAMERA_H
#define CAMERA_H

#include <algorithm>
#include <QPointF>
#include <QRect>

class Camera
{
public:
    static Camera &getInstance();

    QPointF screenToWorld(const QPoint& screenPos) const;

    QPointF hexRound(float q, float r) const;

    void handleEdgePanning(const QPoint &mousePos, int viewWidth, int viewHeight, float deltaTime);

    void setTargetPos(QPointF worldPos);

    QPointF getCurrentPos();

    static constexpr float MIN_ZOOM = 0.75f;

    static constexpr float MAX_ZOOM = 3.0f;

    void update(float deltaTime);

    void move(float dx, float dy);

    void adjustZoom(float delta);

    void setWorldBounds(const QRect &bounds) { m_worldBounds = bounds; }

    void setViewportSize(int w, int h)
    {
        m_viewportWidth = w;
        m_viewportHeight = h;
    }

    QPointF pos() const { return m_currentPos; }

    float getZoom() const { return m_zoom; }

    QPoint toScreen(int tileX, int tileY, int tileSize) const;

    void shake(float intensity) { m_shakeIntensity = intensity; }

private:
    Camera();

    QPointF m_currentPos;

    QPointF m_targetPos;

    float m_zoom = 1.5f;

    QRect m_worldBounds;

    int m_viewportWidth = 800;

    int m_viewportHeight = 600;

    const float m_smoothing = 0.15f;

    float m_shakeIntensity = 0.0f;

    QPointF m_shakeOffset;

    const int EDGE_MARGIN = 15;
    
    const float EDGE_MOVE_SPEED = 10.0f;
};

#endif
