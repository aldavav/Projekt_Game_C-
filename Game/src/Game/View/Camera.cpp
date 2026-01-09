#include "Camera.h"

Camera &Camera::getInstance()
{
    static Camera instance;
    return instance;
}

Camera::Camera() : m_currentPos(0, 0), m_targetPos(0, 0) {}

void Camera::move(float dx, float dy)
{
    m_targetPos.setX(m_targetPos.x() + dx);
    m_targetPos.setY(m_targetPos.y() + dy);
}

#include <random>

void Camera::update(float deltaTime)
{
    float lerpFactor = 1.0f - std::pow(m_smoothing, deltaTime * 60.0f);
    m_currentPos += (m_targetPos - m_currentPos) * lerpFactor;

    if (!m_worldBounds.isNull())
    {
        float minX = static_cast<float>(m_worldBounds.left());
        float maxX = static_cast<float>(m_worldBounds.right());
        m_currentPos.setX(std::clamp(static_cast<float>(m_currentPos.x()), minX, maxX));

        float minY = static_cast<float>(m_worldBounds.top());
        float maxY = static_cast<float>(m_worldBounds.bottom());
        m_currentPos.setY(std::clamp(static_cast<float>(m_currentPos.y()), minY, maxY));
    }

    if (m_shakeIntensity > 0.1f)
    {
        float rx = ((float)rand() / (float)RAND_MAX * 2.0f - 1.0f) * m_shakeIntensity;
        float ry = ((float)rand() / (float)RAND_MAX * 2.0f - 1.0f) * m_shakeIntensity;
        m_shakeOffset = QPointF(rx, ry);
        m_shakeIntensity *= 0.9f;
    }
    else
    {
        m_shakeOffset = QPointF(0, 0);
        m_shakeIntensity = 0.0f;
    }
}

void Camera::adjustZoom(float delta)
{
    float minZ = static_cast<float>(MIN_ZOOM);
    float maxZ = static_cast<float>(MAX_ZOOM);
    m_zoom = std::clamp(m_zoom + delta, minZ, maxZ);
}

QPoint Camera::toScreen(int tileX, int tileY, int tileSize) const
{
    float worldX = tileX * tileSize;
    float worldY = tileY * tileSize;

    float screenX = (worldX - (m_currentPos.x() + m_shakeOffset.x())) * m_zoom + (m_viewportWidth / 2.0f);
    float screenY = (worldY - (m_currentPos.y() + m_shakeOffset.y())) * m_zoom + (m_viewportHeight / 2.0f);

    return QPoint(static_cast<int>(screenX), static_cast<int>(screenY));
}

QPointF Camera::screenToWorld(const QPoint &screenPos) const
{
    float centeredX = static_cast<float>(screenPos.x()) - (m_viewportWidth / 2.0f);
    float centeredY = static_cast<float>(screenPos.y()) - (m_viewportHeight / 2.0f);

    float zoomedX = centeredX / m_zoom;
    float zoomedY = centeredY / m_zoom;

    float worldX = zoomedX + (m_currentPos.x() + m_shakeOffset.x());
    float worldY = zoomedY + (m_currentPos.y() + m_shakeOffset.y());

    return QPointF(worldX, worldY);
}
