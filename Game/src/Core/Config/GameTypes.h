#ifndef GAMETYPES_H
#define GAMETYPES_H

#include <QtGlobal>
#include <QPoint>

namespace Engine
{
    enum class State
    {
        MENU,
        LOADING,
        RUNNING,
        GAMEOVER
    };
    enum class LogLevel
    {
        Info,
        Warning,
        Error
    };
    enum class Difficulty
    {
        EASY,
        MEDIUM,
        HARD,
        BRUTAL
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
            MOVE_UP,
            MOVE_DOWN,
            MOVE_LEFT,
            MOVE_RIGHT,
            STOP,
            GUARD,
            SCATTER
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
}

#endif
