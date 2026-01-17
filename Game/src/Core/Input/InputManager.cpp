#include "InputManager.h"

InputManager &InputManager::getInstance()
{
    static InputManager instance;
    return instance;
}

InputManager::InputManager(QObject *parent) : QObject(parent)
{
    setupDefaultBindings();
}

InputManager::~InputManager() {}

void InputManager::setupDefaultBindings()
{
}

void InputManager::onKeyPress(int keyCode)
{
    bool isRepeat = m_activeKeys.count(keyCode) > 0;
    m_activeKeys.insert(keyCode);

    if (isRepeat)
        return;

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
        switch (event.keyCode)
        {
        case Qt::Key_S:
            return QSharedPointer<StopAction>::create().staticCast<ICommand>();
        case Qt::Key_Minus:
            return QSharedPointer<ZoomAction>::create(-0.1f).staticCast<ICommand>();
        case Qt::Key_Plus:
        case Qt::Key_Equal:
            return qSharedPointerCast<ICommand>(QSharedPointer<ZoomAction>::create(0.1f));
        default:
            break;
        }
    }

    if (event.type == RawInputEvent::Type::MouseClick)
    {
        if (event.button == Qt::LeftButton)
        {
            return qSharedPointerCast<ICommand>(QSharedPointer<MoveUnitAction>::create(event.position));
        }
    }

    return nullptr;
}

CommandPtr InputManager::getNextCommand()
{
    QMutexLocker locker(&m_inputMutex);
    return m_commandQueue.isEmpty() ? nullptr : m_commandQueue.dequeue();
}

bool InputManager::hasPendingCommands() const
{
    return !m_commandQueue.isEmpty();
}
