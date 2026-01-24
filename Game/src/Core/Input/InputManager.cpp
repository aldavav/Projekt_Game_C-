#include "InputManager.h"

InputManager &InputManager::getInstance()
{
    static InputManager instance;
    return instance;
}

bool InputManager::isKeyPressed(int keyCode) const
{
    return m_activeKeys.find(keyCode) != m_activeKeys.end();
}

bool InputManager::hasPendingCommands() const
{
    return !m_commandQueue.isEmpty();
}

CommandPtr InputManager::getNextCommand()
{
    QMutexLocker locker(&m_inputMutex);
    return m_commandQueue.isEmpty() ? nullptr : m_commandQueue.dequeue();
}

InputManager::InputManager(QObject *parent) : QObject(parent)
{
    setupDefaultBindings();
}

void InputManager::onKeyPress(int keyCode)
{
    bool isRepeat = m_activeKeys.count(keyCode) > 0;
    m_activeKeys.insert(keyCode);

    if (isRepeat)
        return;

    Engine::Input::RawEvent event;
    event.type = Engine::Input::RawEvent::Type::Keyboard;
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

void InputManager::onMouseClick(Qt::MouseButton button, const QPoint &pos)
{
    Engine::Input::RawEvent event;
    event.type = Engine::Input::RawEvent::Type::MouseClick;
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
    if (m_activeKeys.count(Qt::RightButton))
    {
        Engine::Input::RawEvent event;
        event.type = Engine::Input::RawEvent::Type::MouseMove;
        event.position = pos;

        QMutexLocker locker(&m_inputMutex);

        CommandPtr command = translateRawInput(event);
        if (command)
        {
            m_commandQueue.enqueue(command);
            emit commandQueued();
        }
    }
}

void InputManager::setupDefaultBindings()
{
    ControlsSettingsManager::getInstance();
}

CommandPtr InputManager::translateRawInput(const Engine::Input::RawEvent &event)
{
    auto &settings = ControlsSettingsManager::getInstance();

    if (event.type == Engine::Input::RawEvent::Type::Keyboard)
    {
        int keyCode = event.keyCode;

        if (keyCode == static_cast<int>(settings.getKey(Engine::Input::Action::STOP)))
            return QSharedPointer<StopUnitAction>::create().staticCast<ICommand>();

        if (keyCode == static_cast<int>(settings.getKey(Engine::Input::Action::GUARD)))
            return QSharedPointer<GuardUnitAction>::create().staticCast<ICommand>();

        if (keyCode == static_cast<int>(settings.getKey(Engine::Input::Action::ZOOM_OUT)))
            return QSharedPointer<ZoomAction>::create(-Config::Gameplay::ZOOM_STEP).staticCast<ICommand>();

        if (keyCode == static_cast<int>(settings.getKey(Engine::Input::Action::ZOOM_IN)) ||
            keyCode == static_cast<int>(settings.getKey(Engine::Input::Action::ZOOM_IN_ALT)))
        {
            return QSharedPointer<ZoomAction>::create(Config::Gameplay::ZOOM_STEP).staticCast<ICommand>();
        }
    }

    if (event.type == Engine::Input::RawEvent::Type::MouseClick)
    {

        if (event.button == Config::Input::BTN_MOVE)
        {
            return QSharedPointer<MoveUnitAction>::create(event.position).staticCast<ICommand>();
        }
    }

    return nullptr;
}
