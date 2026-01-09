#include "CameraActions.h"

bool ZoomAction::execute(GameEngine &engine)
{
    Camera::getInstance().adjustZoom(m_delta);
    return true;
}

bool ZoomAction::undo()
{
    Camera::getInstance().adjustZoom(-m_delta);
    return true;
}
