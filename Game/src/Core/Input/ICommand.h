#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <QSharedPointer>

class GameEngine;

class ICommand
{
public:
    virtual ~ICommand() = default;

    virtual bool execute(GameEngine& engine) = 0;

    virtual bool undo() = 0;
};

using CommandPtr = QSharedPointer<ICommand>;

#endif
