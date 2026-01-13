#ifndef MAINMENUSCREEN_H
#define MAINMENUSCREEN_H

#include <UI/Screens/SettingsScreen.h>
#include <UI/Components/MenuButton.h>
#include <Core/Config/GameConfig.h>
#include <UI/Manager/MenuManager.h>
#include <UI/Screens/GameScreen.h>
#include <QCoreApplication>
#include <QResizeEvent>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QWidget>
#include <QLabel>
#include <QMovie>

class QLabel;
class QMovie;
class QPushButton;
class QVBoxLayout;
class QResizeEvent;

class MainMenuScreen : public AbstractScreen
{
    Q_OBJECT

public:
    explicit MainMenuScreen(QWidget *parent = nullptr);

    virtual ~MainMenuScreen() = default;

    void onEnter() override;

    void onExit() override;

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onNewGameClicked();

    void onLoadGameClicked();

    void onSettingsClicked();

    void onHelpClicked();

    void onCreditsClicked();

    void onQuitClicked();

private:
    QLabel *m_backgroundLabel = nullptr;

    QMovie *m_bgMovie = nullptr;

    QWidget *m_sidePanel = nullptr;

    QLabel *m_panelTitle = nullptr;

    QLabel *m_panelText = nullptr;

    QVBoxLayout *m_buttonLayout = nullptr;

    void setupUI();

    void setupBackground();

    void updatePanelContent(const QString &title, const QString &description);
};

#endif
