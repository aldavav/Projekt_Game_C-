#ifndef MAINMENUSCREEN_H
#define MAINMENUSCREEN_H

#include <Core/Logger/LoggerMacros.h>
#include <QGraphicsDropShadowEffect>
#include <UI/Manager/MenuManager.h>
#include <UI/Screens/GameScreen.h>
#include <QCoreApplication>
#include <QResizeEvent>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScreen>
#include <QLabel>
#include <QFont>
#include <QMovie>

class MainMenuScreen : public AbstractScreen
{
    Q_OBJECT

public:
    explicit MainMenuScreen(QWidget *parent = nullptr);

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
    QLabel *m_backgroundLabel;

    QMovie *m_bgMovie;

    QWidget* m_sidePanel = nullptr;

    QLabel* m_panelTitle = nullptr;
    
    QLabel* m_panelText = nullptr;
};

#endif
