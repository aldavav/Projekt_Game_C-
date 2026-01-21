#ifndef CAMERAACTIONS_H
#define CAMERAACTIONS_H

#include <Core/Common/ICommand.h>

class ZoomAction : public ICommand
{
public:
    explicit ZoomAction(float delta) : m_delta(delta) {}

    bool execute(GameEngine &engine) override;

    bool undo() override;

    QString debugName() const override { return "ZoomAction"; }

private:
    float m_delta;
};

#endif
