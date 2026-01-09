#ifndef KEYCODES_H
#define KEYCODES_H

#include <Qt>

namespace Input
{
    enum class KeyCode : int
    {
        ENTER = Qt::Key_Return,
        ESC = Qt::Key_Escape,
        UP_ARROW = Qt::Key_W,
        DOWN_ARROW = Qt::Key_S,
        LEFT_ARROW = Qt::Key_A,
        RIGHT_ARROW = Qt::Key_D,
        SPACE = Qt::Key_Space,
        STOP = Qt::Key_S,
        GUARD = Qt::Key_G,
        SCATTER = Qt::Key_X
    };
}

#endif
