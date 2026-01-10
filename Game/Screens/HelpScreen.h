#pragma once

#include "MenuManager.h"

class HelpScreen : public AbstractScreen
{
    Q_OBJECT
public:
    explicit HelpScreen(QWidget *parent = nullptr);

    void onEnter() override;
    void onExit() override;
};
