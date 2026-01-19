#ifndef LOADGAMESCREEN_H
#define LOADGAMESCREEN_H

#include <UI/Screens/AbstractScreen.h>
#include <UI/Screens/LoadingScreen.h>
#include <UI/Manager/MenuManager.h>
#include <Game/Engine/GameEngine.h>
#include <UI/Screens/GameScreen.h>
#include <qcoreapplication.h>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QDir>

class LoadGameScreen : public AbstractScreen
{
    Q_OBJECT
public:
    explicit LoadGameScreen(QWidget *parent = nullptr);

    void onEnter() override;

    void onExit() override;

private:
    QListWidget *m_saveList;

    QPushButton *m_loadBtn;

    QLabel *m_detailsLabel;

    void setupUI();

    void refreshSaveList();

    void onLoadClicked();

    void onEntrySelected(QListWidgetItem *item);
};

#endif
