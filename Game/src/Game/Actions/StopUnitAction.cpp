#include "StopUnitAction.h"

bool StopUnitAction::execute(GameEngine &engine)
{
    return true;
}

bool StopUnitAction::undo()
{
    return false;
}
