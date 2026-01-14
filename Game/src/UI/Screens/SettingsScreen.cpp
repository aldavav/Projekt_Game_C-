#include "SettingsScreen.h"

SettingsScreen::SettingsScreen(QWidget *parent) : AbstractScreen(parent)
{
    setObjectName("settingsScreen");
    setupUI();
}

void SettingsScreen::onEnter() { this->show(); }
void SettingsScreen::onExit() { this->hide(); }

void SettingsScreen::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(50, 50, 50, 50);

    m_headerLabel = new QLabel(this);
    m_headerLabel->setObjectName("settingsTitle");
    mainLayout->addWidget(m_headerLabel);

    m_tabs = new QTabWidget(this);
    m_tabs->addTab(createGameTab(), "");
    m_tabs->addTab(createInputTab(), "");
    m_tabs->addTab(createDisplayTab(), "");
    m_tabs->addTab(createGraphicsTab(), "");
    m_tabs->addTab(createAudioTab(), "");

    mainLayout->addWidget(m_tabs);

    QHBoxLayout *btnLayout = new QHBoxLayout();

    QPushButton *resetBtn = new QPushButton(tr("RESET TO DEFAULTS"));
    connect(resetBtn, &QPushButton::clicked, this, &SettingsScreen::onResetClicked);
    btnLayout->addWidget(resetBtn);

    btnLayout->addStretch();

    QPushButton *cancelBtn = new QPushButton(tr("BACK"));
    QPushButton *applyBtn = new QPushButton(tr("APPLY CHANGES"));
    applyBtn->setObjectName("applyButton");

    connect(cancelBtn, &QPushButton::clicked, this, &SettingsScreen::onBackClicked);
    connect(applyBtn, &QPushButton::clicked, this, &SettingsScreen::onApplyClicked);

    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(applyBtn);
    mainLayout->addLayout(btnLayout);

    retranslateUi();
}

void SettingsScreen::retranslateUi()
{
    m_headerLabel->setText(tr("SYSTEM CONFIGURATION"));
    m_tabs->setTabText(0, tr("GAME"));
    m_tabs->setTabText(1, tr("CONTROLS"));
    m_tabs->setTabText(2, tr("DISPLAY"));
    m_tabs->setTabText(3, tr("GRAPHICS"));
    m_tabs->setTabText(4, tr("AUDIO"));
}

void SettingsScreen::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        retranslateUi();
    }
    AbstractScreen::changeEvent(event);
}

QWidget *SettingsScreen::createGameTab()
{
    auto *w = new QWidget();
    auto *layout = new QFormLayout(w);
    auto &cfg = ConfigManager::getInstance().getSettings();

    auto *langCombo = new QComboBox();
    langCombo->addItem("English", "en");
    langCombo->addItem("Čeština", "cz");
    langCombo->setCurrentIndex(cfg.languageIndex);

    auto *tooltipsCheck = new QCheckBox(tr("Enable Context Tooltips"));
    tooltipsCheck->setChecked(cfg.showTooltips);

    layout->addRow(tr("SYSTEM LANGUAGE"), langCombo);
    layout->addRow(tr("USER INTERFACE"), tooltipsCheck);

    connect(langCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, &cfg, langCombo](int index)
            {
        m_isDirty = true;
        cfg.languageIndex = index;
        QString langCode = langCombo->itemData(index).toString();
        GameSettingsManager::getInstance().setLanguage(langCode); });

    connect(tooltipsCheck, &QCheckBox::toggled, this, [this, &cfg](bool checked)
            {
        m_isDirty = true;
        cfg.showTooltips = checked;
        GameSettingsManager::getInstance().setTooltipsEnabled(checked); });

    return w;
}

QWidget *SettingsScreen::createDisplayTab()
{
    auto *w = new QWidget();
    auto *layout = new QFormLayout(w);
    auto &cfg = ConfigManager::getInstance().getSettings();
    auto &dm = DisplaySettingsManager::getInstance();

    auto *resCombo = new QComboBox();
    resCombo->addItems(dm.getAvailableResolutions());
    resCombo->setCurrentIndex(cfg.resolutionIndex);

    auto *winCombo = new QComboBox();
    winCombo->addItems({tr("Fullscreen"), tr("Borderless"), tr("Windowed")});
    winCombo->setCurrentIndex(cfg.windowModeIndex);

    auto *vsyncCheck = new QCheckBox(tr("Limit Frame Rate (VSync)"));
    vsyncCheck->setChecked(cfg.vsync);

    layout->addRow(tr("RESOLUTION"), resCombo);
    layout->addRow(tr("DISPLAY MODE"), winCombo);
    layout->addRow(tr("VERTICAL SYNC"), vsyncCheck);

    connect(resCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, &cfg](int idx)
            {
        m_isDirty = true; cfg.resolutionIndex = idx; });
    connect(winCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, &cfg](int idx)
            {
        m_isDirty = true; cfg.windowModeIndex = idx; });
    connect(vsyncCheck, &QCheckBox::toggled, this, [this, &cfg](bool chk)
            {
        m_isDirty = true; cfg.vsync = chk; });

    return w;
}

QWidget *SettingsScreen::createGraphicsTab()
{
    auto *w = new QWidget();
    auto *layout = new QFormLayout(w);
    auto &cfg = ConfigManager::getInstance().getSettings();

    auto *texCombo = new QComboBox();
    texCombo->addItems({tr("LOW"), tr("MEDIUM"), tr("HIGH"), tr("ULTRA")});
    texCombo->setCurrentIndex(cfg.textureQualityIndex);

    auto *gammaSlider = new QSlider(Qt::Horizontal);
    gammaSlider->setRange(10, 90);
    gammaSlider->setValue(cfg.gamma);

    layout->addRow(tr("TEXTURE QUALITY"), texCombo);
    layout->addRow(tr("GAMMA CORRECTION"), gammaSlider);

    connect(texCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, &cfg](int index)
            {
        m_isDirty = true;
        cfg.textureQualityIndex = index; });

    connect(gammaSlider, &QSlider::valueChanged, this, [this, &cfg](int val)
            {
        m_isDirty = true;
        cfg.gamma = val;
        GraphicsSettingsManager::getInstance().applyGraphicsSettings(); });

    return w;
}

QWidget *SettingsScreen::createAudioTab()
{
    auto *w = new QWidget();
    auto *layout = new QFormLayout(w);
    auto &cfg = ConfigManager::getInstance().getSettings();

    auto *masterSlider = new QSlider(Qt::Horizontal);
    auto *musicSlider = new QSlider(Qt::Horizontal);
    auto *sfxSlider = new QSlider(Qt::Horizontal);
    auto *voiceSlider = new QSlider(Qt::Horizontal);

    QList<QPair<QSlider *, int *>> sliderPairs = {
        {masterSlider, &cfg.masterVol}, {musicSlider, &cfg.musicVol}, {sfxSlider, &cfg.sfxVol}, {voiceSlider, &cfg.voiceVol}};

    for (auto &pair : sliderPairs)
    {
        pair.first->setRange(0, 100);
        pair.first->setValue(*pair.second);
        connect(pair.first, &QSlider::valueChanged, this, [this, pair](int val)
                {
            m_isDirty = true;
            *pair.second = val;
            emit AudioSettingsManager::getInstance().volumesChanged(); });
    }

    layout->addRow(tr("MASTER VOLUME"), masterSlider);
    layout->addRow(tr("MUSIC VOLUME"), musicSlider);
    layout->addRow(tr("SFX VOLUME"), sfxSlider);
    layout->addRow(tr("VOICE VOLUME"), voiceSlider);

    return w;
}

QWidget *SettingsScreen::createInputTab()
{
    auto *w = new QWidget();
    auto *layout = new QFormLayout(w);

    layout->addRow(new QLabel(tr("KEY BINDINGS")));

    auto createBindRow = [this, layout](const QString &label, ControlsSettingsManager::Action action)
    {
        auto *btn = new QPushButton(ControlsSettingsManager::getInstance().getKeyName(action));
        btn->setObjectName("bindButton");

        connect(btn, &QPushButton::clicked, this, [this, btn, action]()
                {
            KeyCaptureDialog diag(this);
            if (diag.exec() == QDialog::Accepted) {
                m_isDirty = true; 
                ControlsSettingsManager::getInstance().setKey(action, static_cast<Input::KeyCode>(diag.capturedKey));
                btn->setText(ControlsSettingsManager::getInstance().getKeyName(action));
            } });
        layout->addRow(label, btn);
    };

    createBindRow(tr("MOVE UP"), ControlsSettingsManager::Action::MOVE_UP);
    createBindRow(tr("MOVE DOWN"), ControlsSettingsManager::Action::MOVE_DOWN);
    createBindRow(tr("UNIT: STOP"), ControlsSettingsManager::Action::STOP);
    createBindRow(tr("UNIT: GUARD"), ControlsSettingsManager::Action::GUARD);
    createBindRow(tr("UNIT: SCATTER"), ControlsSettingsManager::Action::SCATTER);

    return w;
}

void SettingsScreen::onApplyClicked()
{
    ConfigManager::getInstance().saveConfiguration();

    DisplaySettingsManager::getInstance().applySettings();
    GraphicsSettingsManager::getInstance().applyGraphicsSettings();

    m_isDirty = false;
    MenuManager::getInstance().popScreen();
}

void SettingsScreen::onResetClicked()
{
    TacticalDialog confirm(tr("FACTORY RESET"),
                           tr("Restore all settings to default?"),
                           this);

    if (confirm.exec() == QDialog::Accepted)
    {

        ConfigManager::getInstance().resetToDefaults();

        m_isDirty = true;

        QTimer::singleShot(0, this, [this]()
                           {
            
            if (layout()) {
                qDeleteAll(this->children());
            }
            
            setupUI(); });
    }
}

void SettingsScreen::onBackClicked()
{
    if (m_isDirty)
    {
        TacticalDialog confirm(tr("Unsaved Changes"), tr("Discard changes and exit?"), this);
        if (confirm.exec() == QDialog::Rejected)
            return;
        ConfigManager::getInstance().loadConfiguration();
    }
    MenuManager::getInstance().popScreen();
}
