#ifndef LOADGAMESCREEN_H
#define LOADGAMESCREEN_H

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
    void setupUI();

    void refreshSaveList();

    void onEntrySelected(QListWidgetItem *item);

    void onLoadClicked();

    QListWidget *m_saveList;

    QPushButton *m_loadBtn;

    QLabel *m_detailsLabel;
};

#endif
