#include "InputManager.h"

class PlaceholderCommand : public ICommand
{
private:
    QString m_action;

public:
    PlaceholderCommand(const QString &action) : m_action(action) {}
    bool execute() override
    {
        qDebug() << "EXECUTING:" << m_action;
        return true;
    }
    bool undo() override
    {
        qDebug() << "UNDOING:" << m_action;
        return true;
    }
};

InputManager &InputManager::getInstance()
{
    static InputManager instance;
    return instance;
}

InputManager::InputManager(QObject *parent) : QObject(parent) {}

InputManager::~InputManager() {}

void InputManager::onKeyPress(int keyCode)
{
    m_activeKeys.insert(keyCode);

    RawInputEvent event;
    event.type = RawInputEvent::Type::Keyboard;
    event.keyCode = keyCode;

    CommandPtr command = translateRawInput(event);
    if (command)
    {
        m_commandQueue.enqueue(command);
        emit commandQueued();
    }
}

void InputManager::onKeyRelease(int keyCode)
{
    m_activeKeys.erase(keyCode);
}

bool InputManager::isKeyPressed(int keyCode) const
{
    return m_activeKeys.find(keyCode) != m_activeKeys.end();
}

void InputManager::onMouseClick(Qt::MouseButton button, const QPoint &pos)
{
    RawInputEvent event;
    event.type = RawInputEvent::Type::MouseClick;
    event.button = button;
    event.position = pos;

    CommandPtr command = translateRawInput(event);
    if (command)
    {
        m_commandQueue.enqueue(command);
        emit commandQueued();
    }
}

void InputManager::onMouseMove(const QPoint &pos)
{
}

CommandPtr InputManager::translateRawInput(const RawInputEvent &event)
{
    if (event.type == RawInputEvent::Type::Keyboard)
    {
        if (event.keyCode == Qt::Key_B)
            return CommandPtr(new PlaceholderCommand("Open Build Menu"));

        if (event.keyCode == Qt::Key_Escape)
            return CommandPtr(new PlaceholderCommand("Toggle Menu"));
    }

    if (event.type == RawInputEvent::Type::MouseClick)
    {
        if (event.button == Qt::LeftButton)
        {
            QString posText = QString("Pos: %1, %2").arg(event.position.x()).arg(event.position.y());
            return CommandPtr(new PlaceholderCommand("Select at " + posText));
        }
    }

    return nullptr;
}

CommandPtr InputManager::getNextCommand()
{
    return m_commandQueue.isEmpty() ? nullptr : m_commandQueue.dequeue();
}

bool InputManager::hasPendingCommands() const
{
    return !m_commandQueue.isEmpty();
}
