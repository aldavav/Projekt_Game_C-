#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include <UI/Screens/SettingsScreen.h>
#include <UI/Screens/AbstractScreen.h>
#include <Core/Config/Configuration.h>
#include <UI/Components/MenuButton.h>
#include <UI/Screens/NewGameScreen.h>
#include <QCoreApplication>
#include <QResizeEvent>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QWidget>
#include <QLabel>
#include <QMovie>

class QResizeEvent;
class QPushButton;
class QVBoxLayout;
class QLabel;
class QMovie;

class MenuScreen : public AbstractScreen
{
    Q_OBJECT

public:
    explicit MenuScreen(QWidget *parent = nullptr);

    void onEnter() override;

    void onExit() override;

protected:
    void resizeEvent(QResizeEvent *event) override;

    void showEvent(QShowEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onNewGameClicked();

    void onLoadGameClicked();

    void onSettingsClicked();

    void onHelpClicked();

    void onCreditsClicked();

    void onQuitClicked();

private:
    void setupUI();

    void setupBackground();

    void updatePanelContent(const QString &title, const QString &description);
    
    QLabel *m_backgroundLabel = nullptr;

    QMovie *m_bgMovie = nullptr;

    QWidget *m_sidePanel = nullptr;

    QLabel *m_panelTitle = nullptr;

    QLabel *m_panelText = nullptr;

    QVBoxLayout *m_buttonLayout = nullptr;

    QList<MenuButton*> m_buttons;
};

#endif
