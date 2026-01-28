#ifndef NEWGAMESCREEN_H
#define NEWGAMESCREEN_H

#include <UI/Screens/AbstractScreen.h>
#include <UI/Screens/LoadGameScreen.h>
#include <Core/Config/Configuration.h>
#include <UI/Screens/LoadingScreen.h>
#include <UI/Manager/MenuManager.h>
#include <Game/Engine/GameEngine.h>
#include <UI/Screens/GameScreen.h>
#include <QRandomGenerator>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>

class QLineEdit;
class QComboBox;
class QKeyEvent;
class QSpinBox;

class NewGameScreen : public AbstractScreen
{
    Q_OBJECT

public:
    explicit NewGameScreen(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onLaunchClicked();

    void onBackClicked();

private:
    void setupUI();

    QLineEdit *m_mapNameEdit = nullptr;

    QLineEdit *m_seedEdit = nullptr;

    QComboBox *m_difficultyCombo = nullptr;

    QSpinBox *m_aiLevelSpin = nullptr;

    QComboBox *m_mapTypeCombo = nullptr;
};

#endif
