#ifndef GAMETYPES_H
#define GAMETYPES_H

#include <QtGlobal>
#include <QPoint>

namespace Engine
{
    enum class LogLevel
    {
        Info = 0,
        Warning = 1,
        Error = 2
    };
    enum class State
    {
        MENU = 0,
        LOADING = 1,
        RUNNING = 2,
        GAMEOVER = 3
    };
    enum class GameSpeed
    {
        SLOW = 0,
        NORMAL = 1,
        FAST = 2
    };
    enum class Difficulty
    {
        EASY = 0,
        MEDIUM = 1,
        HARD = 2,
        BRUTAL = 3
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
        enum class Type : int
        {
            NONE = 0,
            UNIT = 1,
            BUILDING = 2,
            RESOURCE = 3,
            DEBRIS = 4
        };
    }

    namespace Input
    {
        enum class Action
        {
            MOVE_UP = 0,
            MOVE_DOWN = 1,
            MOVE_LEFT = 2,
            MOVE_RIGHT = 3,
            STOP = 4,
            GUARD = 5,
            SCATTER = 6,
            ZOOM_OUT = 7,
            ZOOM_IN = 8,
            ZOOM_IN_ALT = 9
        };

        enum class KeyCode : int
        {
            UP_ARROW = Qt::Key_Up,
            DOWN_ARROW = Qt::Key_Down,
            LEFT_ARROW = Qt::Key_Left,
            RIGHT_ARROW = Qt::Key_Right,
            STOP = Qt::Key_S,
            GUARD = Qt::Key_G,
            SCATTER = Qt::Key_X,
            ENTER = Qt::Key_Return,
            ESC = Qt::Key_Escape,
            ZOOM_OUT = Qt::Key_Minus,
            ZOOM_IN = Qt::Key_Plus,
            ZOOM_IN_ALT = Qt::Key_Equal
        };

        struct RawEvent
        {
            enum class Type
            {
                Keyboard = 0,
                MouseClick = 1,
                MouseMove = 2
            };
            Type type;
            int keyCode = 0;
            QPoint position;
            Qt::MouseButton button = Qt::NoButton;
        };
    }

    namespace ANSIColors
    {
        constexpr const char *RESET = "\033[0m";
        constexpr const char *RED_BOLD = "\033[1;31m";
        constexpr const char *YELLOW_BOLD = "\033[1;33m";
        constexpr const char *GREEN_NORMAL = "\033[0;32m";
    }
}

#endif
