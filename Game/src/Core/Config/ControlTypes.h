#ifndef CONTROLTYPES_H
#define CONTROLTYPES_H

#include <Core/Common/KeyCodes.h>
#include <QObject>

namespace Controls
{
    Q_NAMESPACE

    using Key = Input::KeyCode;

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

    Q_ENUM_NS(Action)
}

#endif
