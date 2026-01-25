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

    namespace Entity
    {
        enum class Type : uint8_t
        {
            None = 0,
            Unit,
            Building,
            Resource,
            Debris
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
            Stop,
            Guard,
            Scatter,
            ZoomOut,
            ZoomIn,
            ZoomInAlt
        };

        enum class KeyCode : int
        {
            UpArrow = Qt::Key_Up,
            DownArrow = Qt::Key_Down,
            LeftArrow = Qt::Key_Left,
            RightArrow = Qt::Key_Right,
            Stop = Qt::Key_S,
            Guard = Qt::Key_G,
            Scatter = Qt::Key_X,
            Enter = Qt::Key_Return,
            Escape = Qt::Key_Escape,
            ZoomOut = Qt::Key_Minus,
            ZoomIn = Qt::Key_Plus,
            ZoomInAlt = Qt::Key_Equal
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
