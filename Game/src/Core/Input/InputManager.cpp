#include "InputManager.h"

InputManager &InputManager::getInstance()
{
    static InputManager instance;
    return instance;
}

InputManager::InputManager(QObject *parent) : QObject(parent) {}

bool InputManager::isKeyPressed(int keyCode) const
{
    QMutexLocker locker(&m_mutex);
    return m_activeKeys.find(keyCode) != m_activeKeys.end();
}

bool InputManager::hasPendingCommands() const
{
    QMutexLocker locker(&m_mutex);
    return !m_commandQueue.isEmpty();
}

CommandPtr InputManager::getNextCommand()
{
    QMutexLocker locker(&m_mutex);
    return m_commandQueue.isEmpty() ? nullptr : m_commandQueue.dequeue();
}

void InputManager::queueCommand(CommandPtr command)
{
    if (!command)
        return;
    {
        QMutexLocker locker(&m_mutex);
        m_commandQueue.enqueue(command);
    }
    emit commandQueued();
}

void InputManager::onKeyPress(int keyCode)
{
    {
        QMutexLocker locker(&m_mutex);
        if (m_activeKeys.count(keyCode))
            return;
        m_activeKeys.insert(keyCode);
    }

    Engine::Input::RawEvent event;
    event.type = Engine::Input::RawEvent::Type::Keyboard;
    event.keyCode = keyCode;

    queueCommand(translateRawInput(event));
}

void InputManager::onKeyRelease(int keyCode)
{
    QMutexLocker locker(&m_mutex);
    m_activeKeys.erase(keyCode);
}

void InputManager::onMouseClick(Qt::MouseButton button, const QPoint &pos)
{
    Engine::Input::RawEvent event;
    event.type = Engine::Input::RawEvent::Type::MouseClick;
    event.button = button;
    event.position = pos;

    queueCommand(translateRawInput(event));
}

void InputManager::onMouseMove(const QPoint &pos)
{
    if (isKeyPressed(Qt::RightButton))
    {
        Engine::Input::RawEvent event;
        event.type = Engine::Input::RawEvent::Type::MouseMove;
        event.position = pos;
        queueCommand(translateRawInput(event));
    }
}

CommandPtr InputManager::translateRawInput(const Engine::Input::RawEvent &event)
{
    auto &settings = ControlsSettingsManager::getInstance();

    if (event.type == Engine::Input::RawEvent::Type::Keyboard)
    {
        /*auto action = settings.getActionForKey(event.keyCode);

        switch (action)
        {
        case Engine::Input::Action::Stop:
            return CommandPtr::create<StopUnitAction>();
        case Engine::Input::Action::Guard:
            return CommandPtr::create<GuardUnitAction>();
        case Engine::Input::Action::ZoomOut:
            return CommandPtr::create<ZoomAction>(-Config::Gameplay::ZOOM_STEP);
        case Engine::Input::Action::ZoomIn:
        case Engine::Input::Action::ZoomInAlt:
            return CommandPtr::create<ZoomAction>(Config::Gameplay::ZOOM_STEP);
        default:
            break;
        }*/
    }

    if (event.type == Engine::Input::RawEvent::Type::MouseClick)
    {
        if (event.button == Config::Input::BTN_MOVE)
        {
            ///return CommandPtr::create<MoveUnitAction>(event.position);
        }
    }

    return nullptr;
}
