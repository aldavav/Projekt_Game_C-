#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <QSharedPointer>
#include <QDateTime>
#include <QString>

class GameEngine;

class ICommand
{
public:
    virtual bool execute(GameEngine &engine) = 0;

    virtual bool undo(GameEngine &engine) { return false; }

    virtual QString name() const = 0;

    virtual QString description() const { return ""; }

    uint64_t timestamp() const { return m_timestamp; }

protected:
    uint64_t m_timestamp = QDateTime::currentMSecsSinceEpoch();
};

using CommandPtr = QSharedPointer<ICommand>;

#endif
