#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "../KeyCodes.h"
#include "ICommand.h"
#include <QObject>
#include <QQueue>
#include <QPoint>
#include <QDebug>

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

private:
    explicit InputManager(QObject *parent = nullptr);

    ~InputManager() override;

    QQueue<CommandPtr> m_commandQueue;

    CommandPtr translateRawInput(const RawInputEvent &event);

signals:
    void commandQueued();

public:
    static InputManager &getInstance();

    InputManager(const InputManager &) = delete;

    void operator=(const InputManager &) = delete;

    InputManager(InputManager &&) = delete;

    InputManager &operator=(InputManager &&) = delete;

public slots:
    void onKeyPress(int keyCode);

    void onMouseClick(Qt::MouseButton button, const QPoint &pos);

    void onMouseMove(const QPoint &pos);

    CommandPtr getNextCommand();

    bool hasPendingCommands() const;
};

#endif
