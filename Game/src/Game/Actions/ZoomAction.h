#ifndef ZOOMACTION_H
#define ZOOMACTION_H

#include <Core/Common/ICommand.h>
#include <Game/Camera/Camera.h>

class GameEngine;

class ZoomAction : public ICommand
{
public:
    explicit ZoomAction(float delta) : m_zoomDelta(delta) {}

    bool execute(GameEngine &engine) override;

    QString name() const override;

    QString description() const override;

private:
    float m_zoomDelta;
};

#endif
