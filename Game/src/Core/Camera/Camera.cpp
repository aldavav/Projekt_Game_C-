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

    if (m_shakeIntensity > 0.1f) {
        float rx = ((float)rand() / (float)RAND_MAX * 2.0f - 1.0f) * m_shakeIntensity;
        float ry = ((float)rand() / (float)RAND_MAX * 2.0f - 1.0f) * m_shakeIntensity;
        m_shakeOffset = QPointF(rx, ry);

        m_shakeIntensity *= 0.9f; 
    } else {
        m_shakeOffset = QPointF(0, 0);
        m_shakeIntensity = 0.0f;
    }
}

QPoint Camera::toScreen(int tileX, int tileY, int tileSize) const
{
    int zoomTile = static_cast<int>(tileSize * m_zoom);
    
    float finalX = (m_currentPos.x() + m_shakeOffset.x()) * m_zoom;
    float finalY = (m_currentPos.y() + m_shakeOffset.y()) * m_zoom;

    return QPoint(
        (tileX * zoomTile) - static_cast<int>(finalX),
        (tileY * zoomTile) - static_cast<int>(finalY)
    );
}

void Camera::adjustZoom(float delta)
{
    m_zoom = std::clamp(m_zoom + delta, MIN_ZOOM, MAX_ZOOM);
}
