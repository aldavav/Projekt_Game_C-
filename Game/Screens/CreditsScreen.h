#pragma once

#include "MenuManager.h"

class CreditsScreen : public AbstractScreen
{
public:
    explicit CreditsScreen(QWidget *parent = nullptr);

    void onEnter() override;
    void onExit() override;
};
