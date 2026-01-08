#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <Core/Common/KeyCodes.h>
#include "ICommand.h"
#include <QObject>
#include <QQueue>
#include <QPoint>
#include <QDebug>
#include <set>

struct RawInputEvent
{
    enum class Type
    {
        Keyboard,
        MouseClick,
        MouseMove
    };
    Type type;
    int keyCode = 0;
    QPoint position;
    Qt::MouseButton button = Qt::NoButton;
};

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
};

#endif
