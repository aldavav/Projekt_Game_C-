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

void Camera::update(float deltaTime)
{
    float lerpFactor = 1.0f - std::pow(m_smoothing, deltaTime * 60.0f);
    m_currentPos += (m_targetPos - m_currentPos) * lerpFactor;

    if (!m_worldBounds.isNull())
    {
        float minX = static_cast<float>(m_worldBounds.left());
        float maxX = static_cast<float>(m_worldBounds.right());
        float minY = static_cast<float>(m_worldBounds.top());
        float maxY = static_cast<float>(m_worldBounds.bottom());

        m_currentPos.setX(std::clamp(static_cast<float>(m_currentPos.x()), minX, maxX));
        m_currentPos.setY(std::clamp(static_cast<float>(m_currentPos.y()), minY, maxY));

        m_targetPos.setX(std::clamp(static_cast<float>(m_targetPos.x()), minX, maxX));
        m_targetPos.setY(std::clamp(static_cast<float>(m_targetPos.y()), minY, maxY));
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

QPoint Camera::toScreen(int q, int r, int tileSize) const
{
    float size = static_cast<float>(tileSize);

    float worldX = size * (3.0f / 2.0f * q);
    float worldY = size * (std::sqrt(3.0f) / 2.0f * q + std::sqrt(3.0f) * r);

    float screenX = (worldX - m_currentPos.x()) * m_zoom + (m_viewportWidth / 2.0f);
    float screenY = (worldY - m_currentPos.y()) * m_zoom + (m_viewportHeight / 2.0f);

    return QPoint(static_cast<int>(screenX), static_cast<int>(screenY));
}

QPointF Camera::screenToWorld(const QPoint &screenPos) const
{
    float worldX = (screenPos.x() - m_viewportWidth / 2.0f) / m_zoom + m_currentPos.x();
    float worldY = (screenPos.y() - m_viewportHeight / 2.0f) / m_zoom + m_currentPos.y();

    return QPointF(worldX, worldY);
}

QPointF Camera::hexRound(float q, float r) const
{
    float x = q;
    float z = r;
    float y = -x - z;

    int rx = std::round(x);
    int ry = std::round(y);
    int rz = std::round(z);

    float x_diff = std::abs(rx - x);
    float y_diff = std::abs(ry - y);
    float z_diff = std::abs(rz - z);

    if (x_diff > y_diff && x_diff > z_diff)
    {
        rx = -ry - rz;
    }
    else if (y_diff > z_diff)
    {
        ry = -rx - rz;
    }
    else
    {
        rz = -rx - ry;
    }

    return QPointF(rx, rz);
}

void Camera::handleEdgePanning(const QPoint &mousePos, int viewWidth, int viewHeight, float deltaTime)
{
    float dx = 0;
    float dy = 0;

    float moveAmount = (500.0f / m_zoom) * deltaTime;

    if (mousePos.x() < 0 || mousePos.y() < 0 ||
        mousePos.x() > viewWidth || mousePos.y() > viewHeight)
        return;

    if (mousePos.x() < EDGE_MARGIN)
        dx = -moveAmount;
    else if (mousePos.x() > viewWidth - EDGE_MARGIN)
        dx = moveAmount;

    if (mousePos.y() < EDGE_MARGIN)
        dy = -moveAmount;
    else if (mousePos.y() > viewHeight - EDGE_MARGIN)
        dy = moveAmount;

    if (dx != 0 || dy != 0)
    {
        move(dx, dy);
    }
}

void Camera::setTargetPos(QPointF hexCoords)
{
    const float size = 32.0f;

    float x = size * (3.0f / 2.0f * hexCoords.x());
    float y = size * (std::sqrt(3.0f) / 2.0f * hexCoords.x() + std::sqrt(3.0f) * hexCoords.y());

    m_targetPos = QPointF(x, y);
}

void Camera::setTargetRawPos(QPointF worldPos)
{
    m_targetPos = worldPos;
}

QPointF Camera::getCurrentPos()
{
    return m_currentPos;
}

QPoint Camera::screenToHex(const QPoint &screenPos) const
{
    QPointF worldPos = screenToWorld(screenPos);

    const float size = 32.0f;

    float q = (2.0f / 3.0f * worldPos.x()) / size;
    float r = (-1.0f / 3.0f * worldPos.x() + std::sqrt(3.0f) / 3.0f * worldPos.y()) / size;

    QPointF rounded = hexRound(q, r);
    return QPoint(static_cast<int>(rounded.x()), static_cast<int>(rounded.y()));
}
