#ifndef GAMEACTION_H
#define GAMEACTION_H

#include <QSharedPointer>
#include <QString>

class GameEngine;

class GameAction
{
public:
    virtual ~GameAction() = default;

    virtual bool execute(GameEngine &engine) = 0;

    virtual bool undo() { return false; }

    virtual QString debugName() const = 0;
};

using ICommand = GameAction;
using CommandPtr = QSharedPointer<GameAction>;

#endif
