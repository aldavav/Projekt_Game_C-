#ifndef SETTINGSSCREEN_H
#define SETTINGSSCREEN_H

#include <UI/Manager/GraphicsSettingsManager.h>
#include <UI/Manager/ControlsSettingsManager.h>
#include <UI/Manager/DisplaySettingsManager.h>
#include <UI/Manager/AudioSettingsManager.h>
#include <UI/Manager/GameSettingsManager.h>
#include <UI/Widgets/KeyCaptureDialog.h>
#include <UI/Widgets/TacticalDialog.h>
#include <UI/Manager/ConfigManager.h>
#include <UI/Manager/MenuManager.h>
#include <Core/Common/KeyCodes.h>
#include <qpushbutton.h>
#include <qformlayout.h>
#include <qtabwidget.h>
#include <qcombobox.h>
#include <QMessageBox>
#include <qcheckbox.h>
#include <qslider.h>
#include <qlabel.h>

class QTabWidget;
class QSlider;
class QCheckBox;
class QVBoxLayout;

class SettingsScreen : public AbstractScreen
{
    Q_OBJECT
public:
    explicit SettingsScreen(QWidget *parent = nullptr);

    virtual ~SettingsScreen() = default;

    void onEnter() override;

    void onExit() override;

private:
    bool m_isDirty = false;

    QTabWidget *m_tabs = nullptr;

    QLabel *m_headerLabel = nullptr;

    void setupUI();

    void retranslateUi();

    void changeEvent(QEvent *event);

    QWidget *createGameTab();

    QWidget *createDisplayTab();

    QWidget *createGraphicsTab();

    QWidget *createAudioTab();

    QWidget *createInputTab();

private slots:
    void onApplyClicked();

    void onResetClicked();

    void onBackClicked();
};

#endif
