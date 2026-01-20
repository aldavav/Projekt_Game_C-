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
    return true;
}

bool MoveUnitAction::undo()
{
    return false;
}
