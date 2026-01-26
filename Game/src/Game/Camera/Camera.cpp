#include "Camera.h"

Camera &Camera::getInstance()
{
    static Camera instance;
    return instance;
}

void Camera::update(float deltaTime)
{
    float lerpFactor = 1.0f - std::pow(Config::World::CAMERA_SMOOTHING, deltaTime * 60.0f);
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

    if (m_shakeIntensity > Config::World::SHAKE_THRESHOLD)
    {
        float rx = ((float)rand() / (float)RAND_MAX * 2.0f - 1.0f) * m_shakeIntensity;
        float ry = ((float)rand() / (float)RAND_MAX * 2.0f - 1.0f) * m_shakeIntensity;
        m_shakeOffset = QPointF(rx, ry);
        m_shakeIntensity *= Config::World::SHAKE_DECAY;
    }
    else
    {
        m_shakeOffset = QPointF(0, 0);
        m_shakeIntensity = 0.0f;
    }
}

QPointF Camera::screenToWorld(const QPoint &screenPos, bool is3D) const
{
    float dx = screenPos.x() - (m_viewportWidth / 2.0f);
    float dy = screenPos.y() - (m_viewportHeight / 2.0f);

    float relX = dx / m_zoom;
    float relY = dy / m_zoom;

    if (is3D)
    {
        relY *= 2.0f;
    }

    float worldX = relX + m_currentPos.x();
    float worldY = relY + m_currentPos.y();

    return QPointF(worldX, worldY);
}

QPoint Camera::screenToHex(const QPoint &screenPos, bool is3D) const
{
    QPointF worldPos = screenToWorld(screenPos, is3D);

    const float size = Config::World::BASE_TILE_SIZE;
    float q, r;

    if (is3D)
    {
        float correctedY = worldPos.y() + Config::World::HEIGHT_OFFSET;

        q = (2.0f / 3.0f * worldPos.x()) / size;
        r = (-1.0f / 3.0f * worldPos.x() + std::sqrt(3.0f) / 3.0f * correctedY) / size;
    }
    else
    {
        q = (2.0f / 3.0f * worldPos.x()) / size;
        r = (-1.0f / 3.0f * worldPos.x() + std::sqrt(3.0f) / 3.0f * worldPos.y()) / size;
    }

    QPointF rounded = hexRound(q, r);
    return QPoint(static_cast<int>(rounded.x()), static_cast<int>(rounded.y()));
}

QPoint Camera::toScreen(int q, int r, int tileSize, bool is3D) const
{
    float size = static_cast<float>(tileSize);

    float worldX = size * (3.0f / 2.0f * q);
    float worldY = size * (std::sqrt(3.0f) / 2.0f * q + std::sqrt(3.0f) * r);

    float screenX = (worldX - m_currentPos.x()) * m_zoom + (m_viewportWidth / 2.0f);
    float screenY_Relative = (worldY - m_currentPos.y());

    if (is3D)
        screenY_Relative *= Config::World::HEX_Y_SQUASH;

    float screenY = (screenY_Relative * m_zoom) + (m_viewportHeight / 2.0f);

    return QPoint(static_cast<int>(screenX + m_shakeOffset.x()),
                  static_cast<int>(screenY + m_shakeOffset.y()));
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

void Camera::move(float dx, float dy)
{
    m_targetPos.setX(m_targetPos.x() + dx);
    m_targetPos.setY(m_targetPos.y() + dy);
}

void Camera::setTargetPos(QPointF hexCoords)
{
    const float size = Config::World::BASE_TILE_SIZE;

    float x = size * (3.0f / 2.0f * hexCoords.x());
    float y = size * (std::sqrt(3.0f) / 2.0f * hexCoords.x() + std::sqrt(3.0f) * hexCoords.y());

    m_targetPos = QPointF(x, y);
}

void Camera::adjustZoom(float delta)
{
    float minZ = static_cast<float>(Config::World::MIN_ZOOM);
    float maxZ = static_cast<float>(Config::World::MAX_ZOOM);
    m_zoom = std::clamp(m_zoom + delta, minZ, maxZ);
}

void Camera::setViewportSize(int w, int h)
{
    m_viewportWidth = static_cast<float>(w);
    m_viewportHeight = static_cast<float>(h);
}

void Camera::setZoom(float newZoom)
{
    m_zoom = std::clamp(newZoom,
                        Config::World::MIN_ZOOM,
                        Config::World::MAX_ZOOM);
}
