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

class QListWidgetItem;
class QPushButton;
class QListWidget;
class QKeyEvent;
class QLabel;

class LoadGameScreen : public AbstractScreen
{
    Q_OBJECT

public:
    explicit LoadGameScreen(QWidget *parent = nullptr);

    void onEnter() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onLoadClicked();

    void onBackClicked();

    void onEntrySelected(QListWidgetItem *item);

private:
    void setupUI();

    void refreshSaveList();

    QListWidget *m_saveList = nullptr;

    QPushButton *m_loadBtn = nullptr;

    QLabel *m_detailsLabel = nullptr;
};

#endif
