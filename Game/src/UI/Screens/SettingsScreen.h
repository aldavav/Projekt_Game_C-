#ifndef SETTINGSSCREEN_H
#define SETTINGSSCREEN_H

#include <Core/Settings/GraphicsSettingsManager.h>
#include <Core/Settings/ControlsSettingsManager.h>
#include <Core/Settings/DisplaySettingsManager.h>
#include <Core/Settings/AudioSettingsManager.h>
#include <Core/Settings/GameSettingsManager.h>
#include <UI/Widgets/KeyCaptureDialog.h>
#include <Game/Resources/ConfigManager.h>
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
class QTabWidget;
class QCheckBox;
class QSlider;

class SettingsScreen : public AbstractScreen
{
    Q_OBJECT

public:
    explicit SettingsScreen(QWidget *parent = nullptr);

    virtual ~SettingsScreen() = default;

private:
    bool m_isDirty = false;

    QTabWidget *m_tabs = nullptr;

    QLabel *m_headerLabel = nullptr;

    void setupUI();

    void retranslateUi();

    void changeEvent(QEvent *event);

    void markDirty();

    QWidget *createGameTab();

    QWidget *createDisplayTab();

    QWidget *createGraphicsTab();

    QWidget *createAudioTab();

    QWidget *createInputTab();

private slots:
    void onApplyClicked();

    void onResetClicked();

    void onBackClicked();

    void onBindButtonClicked(const QString &actionName);

    void keyPressEvent(QKeyEvent *event);
};

#endif
