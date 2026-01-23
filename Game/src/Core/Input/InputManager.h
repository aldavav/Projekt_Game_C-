#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <Core/Settings/ControlsSettingsManager.h>
#include <Game/Actions/GuardUnitAction.h>
#include <Game/Actions/StopUnitAction.h>
#include <Game/Actions/MoveUnitAction.h>
#include <Game/Actions/CameraActions.h>
#include <Core/Config/Configuration.h>
#include <Core/Common/ICommand.h>
#include <qmutex.h>
#include <QObject>
#include <QQueue>
#include <QPoint>
#include <QDebug>
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

    void setupDefaultBindings();

    CommandPtr translateRawInput(const Engine::Input::RawEvent &event);

    mutable QMutex m_inputMutex;

    QQueue<CommandPtr> m_commandQueue;

    std::set<int> m_activeKeys;

    std::map<int, QString> m_keyBindings;
};

#endif
