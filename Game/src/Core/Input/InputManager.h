#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <UI/Manager/ControlsSettingsManager.h>
#include <Game/Actions/CameraActions.h>
#include <Core/Config/RawInputEvent.h>
#include <Core/Config/GameConfig.h>
#include <Core/Config/ICommand.h>
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

    InputManager(const InputManager &) = delete;

    void operator=(const InputManager &) = delete;

    bool isKeyPressed(int keyCode) const;

    CommandPtr getNextCommand();

    bool hasPendingCommands() const;

public slots:
    void onKeyPress(int keyCode);

    void onKeyRelease(int keyCode);

    void onMouseClick(Qt::MouseButton button, const QPoint &pos);

    void onMouseMove(const QPoint &pos);

signals:
    void commandQueued();

private:
    explicit InputManager(QObject *parent = nullptr);

    ~InputManager() override;

    CommandPtr translateRawInput(const RawInputEvent &event);

    QQueue<CommandPtr> m_commandQueue;

    std::set<int> m_activeKeys;

    std::map<int, QString> m_keyBindings;

    mutable QMutex m_inputMutex;

    void setupDefaultBindings();
};

#endif
