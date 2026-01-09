#include "UnitActions.h"

bool StopAction::execute(GameEngine &engine)
{

    return true;
}

bool StopAction::undo()
{
    return false;
}

bool MoveUnitAction::execute(GameEngine &engine)
{

    QPointF worldPos = Camera::getInstance().screenToWorld(m_target);

    return true;
}

bool MoveUnitAction::undo()
{
    return false;
}
