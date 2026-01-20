#ifndef GUARDUNITACTIONS_H
#define GUARDUNITACTIONS_H

#include <Core/Config/GameActions.h>

class GuardAction : public GameAction
{
public:
    bool execute(GameEngine &engine) override;

    bool undo() override;
};

#endif
