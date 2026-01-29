#ifndef SETTINGSSCREEN_H
#define SETTINGSSCREEN_H

#include <Core/Settings/GraphicsSettingsManager.h>
#include <Core/Settings/ControlsSettingsManager.h>
#include <Core/Settings/DisplaySettingsManager.h>
#include <Core/Settings/AudioSettingsManager.h>
#include <Core/Settings/GameSettingsManager.h>
#include <Game/Resources/ConfigManager.h>
#include <UI/Widgets/KeyCaptureDialog.h>
#include <Core/Config/Configuration.h>
#include <UI/Screens/AbstractScreen.h>
#include <UI/Widgets/TacticalDialog.h>
#include <UI/Manager/MenuManager.h>
#include <qpushbutton.h>
#include <qformlayout.h>
#include <qtabwidget.h>
#include <qcombobox.h>
#include <QMessageBox>
#include <qcheckbox.h>
#include <qslider.h>
#include <qlabel.h>

class QVBoxLayout;
class QPushButton;
class QTabWidget;
class QCheckBox;
class QKeyEvent;
class QSlider;
class QLabel;

class SettingsScreen : public AbstractScreen
{
    Q_OBJECT

public:
    explicit SettingsScreen(QWidget *parent = nullptr);

protected:
    void changeEvent(QEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onApplyClicked();

    void onResetClicked();

    void onBackClicked();

    void onBindButtonClicked(const QString &actionName);

private:
    void setupUI();

    void retranslateUi();

    QWidget *createGameTab();

    QWidget *createDisplayTab();

    QWidget *createGraphicsTab();

    QWidget *createAudioTab();

    QWidget *createInputTab();

    void markDirty();

    QTabWidget *m_tabs = nullptr;

    QLabel *m_headerLabel = nullptr;

    bool m_isDirty = false;

    QMap<Engine::Input::Action, QPushButton *> m_bindButtons;
};

#endif
