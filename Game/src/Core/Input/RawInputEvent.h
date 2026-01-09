#ifndef RAWINPUTEVENT_H
#define RAWINPUTEVENT_H

#include <QPoint>
#include <Qt>

struct RawInputEvent
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

#endif
