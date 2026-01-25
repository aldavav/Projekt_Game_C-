#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Core/Settings/DisplaySettingsManager.h>
#include <UI/Widgets/InformationDialog.h>
#include <Game/Resources/AssetManager.h>
#include <UI/Widgets/TacticalDialog.h>
#include <Core/Config/Configuration.h>
#include <Core/Config/Configuration.h>
#include <UI/Manager/MenuManager.h>
#include <UI/Screens/MenuScreen.h>
#include <qguiapplication.h>
#include <qfontdatabase.h>
#include <QFontDatabase>
#include <QApplication>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QFile>

class QResizeEvent;
class QCloseEvent;
class QMouseEvent;
class QKeyEvent;
class QWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void toggleFullscreen();

    void applyCustomCursor(const QString &path);

    void updateMusicVolume();

    void setupDisplayConnections();

    void applyDisplaySettings();

    void updateWindowMetadata();

protected:
    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_actionQuit_triggered();

private:
    void setupBackgroundMusic();

    void applyGlobalStyles();

    void centerOnScreen();

    QWidget *m_centralWidget = nullptr;

    bool m_isFullscreen = false;

    QMediaPlayer *m_bgmPlayer = nullptr;

    QAudioOutput *m_audioOutput = nullptr;
};

#endif
