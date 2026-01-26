#include "InputManager.h"

InputManager &InputManager::getInstance()
{
    static InputManager instance;
    return instance;
}

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
        Engine::Input::Action action = settings.getActionForKey(event.keyCode);

        switch (action)
        {
        case Engine::Input::Action::Stop:
            return QSharedPointer<StopUnitAction>::create(0, QPoint(0, 0));
        case Engine::Input::Action::Guard:
            return QSharedPointer<GuardUnitAction>::create(0, QPoint(0, 0));
        case Engine::Input::Action::ZoomOut:
            return QSharedPointer<ZoomAction>::create(-Config::Gameplay::ZOOM_STEP);
        case Engine::Input::Action::ZoomIn:
            return QSharedPointer<ZoomAction>::create(Config::Gameplay::ZOOM_STEP);
        case Engine::Input::Action::Scatter:
            return QSharedPointer<ScatterUnitAction>::create(0, QPoint(0, 0));
        default:
            break;
        }
    }

    if (event.type == Engine::Input::RawEvent::Type::MouseClick)
    {
        if (event.button == Config::Input::BTN_MOVE)
        {
            return QSharedPointer<MoveUnitAction>::create(0, event.position);
        }
    }

    return nullptr;
}
