#ifndef MAINMENUSCREEN_H
#define MAINMENUSCREEN_H

#include "../MenuManager/MenuManager.h"
#include <QCoreApplication>
#include <LoggerMacros.h>
#include "GameScreen.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QScreen>
#include <QLabel>
#include <QFont>

class MainMenuScreen : public AbstractScreen
{
    Q_OBJECT

public:
    explicit MainMenuScreen(QWidget *parent = nullptr);

    ~MainMenuScreen() override;

    void onEnter() override;

    void onExit() override;

private slots:
    void onNewGameClicked();

    void onLoadGameClicked();

    void onSettingsClicked();

    void onHelpClicked();

    void onCreditsClicked();

    void onQuitClicked();
};

#endif
