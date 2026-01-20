#ifndef MOVEUNITACTIONS_H
#define MOVEUNITACTIONS_H

#include <Core/Config/GameActions.h>

class StopAction : public GameAction
{
public:
    bool execute(GameEngine &engine) override;

    bool undo() override;
};

#endif
