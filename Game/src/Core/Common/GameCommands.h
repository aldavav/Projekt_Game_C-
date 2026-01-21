#ifndef GAMECOMMANDS_H
#define GAMECOMMANDS_H

#include <Game/Engine/GameEngine.h>
#include <QString>
#include <QDebug>

class DebugAction : public ICommand
{
private:
    QString m_action;

public:
    explicit DebugAction(const QString &action) : m_action(action) {}

    bool execute(GameEngine &engine) override
    {
        qDebug() << "EXECUTING ACTION:" << m_action;
        return true;
    }

    bool undo() override
    {
        return true;
    }

    QString debugName() const override { return "DebugAction"; }
};

#endif
