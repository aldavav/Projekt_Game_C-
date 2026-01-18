#ifndef GAMEOVERSCREEN_H
#define GAMEOVERSCREEN_H

#include <UI/Screens/AbstractScreen.h>
#include <Game/Engine/GameEngine.h>
#include <Core/Config/GameConfig.h>
#include <Core/Config/Config.h>
#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class GameOverScreen : public AbstractScreen
{
    Q_OBJECT

public:
    explicit GameOverScreen(bool victory, QWidget *parent = nullptr);

    void onEnter() override;

    void onExit() override;

private:
    void setupUI(bool victory);

    QLabel *m_titleLabel;

    QLabel *m_statsLabel;
};

#endif
