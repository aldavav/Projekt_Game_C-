#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <Core/Settings/ControlsSettingsManager.h>
#include <Game/Actions/ScatterUnitAction.h>
#include <Game/Actions/GuardUnitAction.h>
#include <Game/Actions/MoveUnitAction.h>
#include <Game/Actions/StopUnitAction.h>
#include <Core/Config/Configuration.h>
#include <Game/Actions/ZoomAction.h>
#include <QObject>
#include <QMutex>
#include <QQueue>
#include <QPoint>
#include <set>

class InputManager : public QObject
{
    Q_OBJECT

public:
    static InputManager &getInstance();

    bool isKeyPressed(int keyCode) const;

    bool hasPendingCommands() const;

    CommandPtr getNextCommand();

public slots:
    void onKeyPress(int keyCode);

    void onKeyRelease(int keyCode);

    void onMouseClick(Qt::MouseButton button, const QPoint &pos);

    void onMouseMove(const QPoint &pos);

signals:
    void commandQueued();

private:
    explicit InputManager(QObject *parent = nullptr);

    void queueCommand(CommandPtr command);

    CommandPtr translateRawInput(const Engine::Input::RawEvent &event);

    mutable QMutex m_mutex;

    QQueue<CommandPtr> m_commandQueue;

    std::set<int> m_activeKeys;
};

#endif
