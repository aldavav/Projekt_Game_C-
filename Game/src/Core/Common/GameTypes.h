#ifndef GAMETYPES_H
#define GAMETYPES_H

#include <QMetaType>
#include <QtGlobal>
#include <QPoint>

namespace Engine
{
    enum class LogLevel
    {
        Info,
        Warning,
        Error
    };

    enum class GameState
    {
        Menu,
        Loading,
        Running,
        GameOver
    };

    enum class GameSpeed
    {
        Slow,
        Normal,
        Fast
    };

    enum class Difficulty
    {
        Easy,
        Medium,
        Hard,
        Brutal
    };

    enum class EntityType : uint8_t
    {
        Unit,
        Building,
        Resource,
        Debris
    };

    namespace Graphics
    {
        enum class Quality
        {
            Low,
            Medium,
            High,
            Ultra
        };

        enum class CursorType
        {
            Standard,
            Attack,
            Select,
            Wait
        };
    }

    namespace Input
    {
        enum class Action
        {
            MoveUp,
            MoveDown,
            MoveLeft,
            MoveRight,
            ZoomOut,
            ZoomIn,
            ToggleMapMode,
            ToggleDebug,
            QuickSave
        };

        enum class KeyCode : int
        {
            W = Qt::Key_W,
            A = Qt::Key_A,
            S = Qt::Key_S,
            D = Qt::Key_D,
            Escape = Qt::Key_Escape,
            ZoomOut = Qt::Key_Minus,
            ZoomIn = Qt::Key_Plus,
            MapMode = Qt::Key_M,
            DebugF3 = Qt::Key_F3,
            SaveF5 = Qt::Key_F5
        };

        struct RawEvent
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
    }

    namespace ConsoleColor
    {
        using ColorCode = const char *;
        constexpr ColorCode Reset = "\033[0m";
        constexpr ColorCode RedBold = "\033[1;31m";
        constexpr ColorCode YellowBold = "\033[1;33m";
        constexpr ColorCode GreenNormal = "\033[0;32m";
    }
}

Q_DECLARE_METATYPE(Engine::GameState)
Q_DECLARE_METATYPE(Engine::GameSpeed)

#endif
