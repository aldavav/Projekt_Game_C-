#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <UI/Screens/MainMenuScreen.h>
#include <UI/Widgets/TacticalDialog.h>
#include <Core/Logger/LoggerMacros.h>
#include <Core/Assets/AssetManager.h>
#include <Core/Config/GameConfig.h>
#include <qguiapplication.h>
#include <qfontdatabase.h>
#include <QFontDatabase>
#include <QApplication>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QFile>

class QKeyEvent;
class QMouseEvent;
class QResizeEvent;
class QCloseEvent;
class QWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    virtual ~MainWindow() = default;

    void toggleFullscreen();

    void applyCustomCursor(const QString &path);

protected:
    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_actionQuit_triggered();

    void updateWindowMetadata();

private:
    QWidget *m_centralWidget;

    bool m_isFullscreen = false;

    QMediaPlayer *m_bgmPlayer;

    QAudioOutput *m_audioOutput;

    void setupBackgroundMusic();

    void applyGlobalStyles();

    void centerOnScreen();
};

#endif
