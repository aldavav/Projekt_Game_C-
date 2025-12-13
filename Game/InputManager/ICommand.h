#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <QSharedPointer>

class ICommand
{
public:
    virtual ~ICommand() = default;

    virtual bool execute() = 0;

    virtual bool undo() = 0;
};

using CommandPtr = QSharedPointer<ICommand>;

#endif
