#ifndef NEWGAMESCREEN_H
#define NEWGAMESCREEN_H

#include <UI/Screens/AbstractScreen.h>
#include <UI/Screens/LoadGameScreen.h>
#include <UI/Screens/LoadingScreen.h>
#include <UI/Manager/MenuManager.h>
#include <Game/Engine/GameEngine.h>
#include <UI/Screens/GameScreen.h>
#include <Core/Config/Config.h>
#include <QRandomGenerator>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>

class NewGameScreen : public AbstractScreen
{
    Q_OBJECT

public:
    explicit NewGameScreen(QWidget *parent = nullptr);

    virtual ~NewGameScreen() = default;

    void onEnter() override;

    void onExit() override;

private:
    void setupUI();

    void onLaunchClicked();

    void onBackClicked();

    QLineEdit *m_mapNameEdit;

    QLineEdit *m_seedEdit;

    QComboBox *m_difficultyCombo;

    QComboBox *m_opponentCombo;

    QSpinBox *m_aiLevelSpin;

    void keyPressEvent(QKeyEvent *event) override;
};

#endif
