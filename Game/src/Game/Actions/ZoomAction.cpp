#include "ZoomAction.h"

bool ZoomAction::execute(GameEngine &engine)
{
    auto &cam = Camera::getInstance();

    float currentZoom = cam.getZoom();
    float newZoom = currentZoom + (m_zoomDelta * Config::Gameplay::ZOOM_STEP);

    newZoom = std::clamp(newZoom, Config::World::MIN_ZOOM, Config::World::MAX_ZOOM);

    cam.setZoom(newZoom);
    return true;
}

QString ZoomAction::name() const
{
    return "CameraZoom";
}

QString ZoomAction::description() const
{
    return QString("Adjusting camera zoom by %1").arg(m_zoomDelta);
}
