#include "InputManager.h"

class PlaceholderCommand : public ICommand
{
private:
    QString m_action;

public:
    PlaceholderCommand(const QString &action) : m_action(action) {}
    bool execute() override
    {
        qDebug() << "EXECUTING Command:" << m_action;
        return true;
    }
    bool undo() override
    {
        qDebug() << "UNDOING Command:" << m_action;
        return true;
    }
};

InputManager &InputManager::getInstance()
{
    static InputManager instance;
    return instance;
}

InputManager::InputManager(QObject *parent)
    : QObject(parent)
{
    qDebug() << "InputManager initialized.";
}

InputManager::~InputManager()
{
    qDebug() << "InputManager shut down.";
}

void InputManager::onKeyPress(int keyCode)
{
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
    switch (event.type)
    {
    case RawInputEvent::Type::Keyboard:
        qDebug() << "Translating Key Code:" << event.keyCode;
        if (event.keyCode == Qt::Key_S)
        {
            return CommandPtr(new PlaceholderCommand("Select All Units"));
        }
        if (event.keyCode == Qt::Key_B)
        {
            return CommandPtr(new PlaceholderCommand("Open Build Menu"));
        }
        break;
    case RawInputEvent::Type::MouseClick:
        qDebug() << "Translating Mouse Click at:" << event.position << "Button:" << event.button;
        if (event.button == Qt::LeftButton)
        {
            return CommandPtr(new PlaceholderCommand("Primary Selection at " + QString::number(event.position.x())));
        }
        if (event.button == Qt::RightButton)
        {
            return CommandPtr(new PlaceholderCommand("Issue Move/Attack Order at " + QString::number(event.position.y())));
        }
        break;
    default:
        break;
    }
    return CommandPtr(nullptr);
}

CommandPtr InputManager::getNextCommand()
{
    if (m_commandQueue.isEmpty())
    {
        return CommandPtr(nullptr);
    }
    return m_commandQueue.dequeue();
}

bool InputManager::hasPendingCommands() const
{
    return !m_commandQueue.isEmpty();
}
