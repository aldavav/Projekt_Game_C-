#include "SettingsScreen.h"

SettingsScreen::SettingsScreen(QWidget *parent) : AbstractScreen(parent)
{
    setupUI();
}

void SettingsScreen::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        retranslateUi();
    }
    AbstractScreen::changeEvent(event);
}

void SettingsScreen::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        onBackClicked();
    }
    else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        if (!focusWidget() || !focusWidget()->inherits("QPushButton"))
        {
            onApplyClicked();
        }
    }
    else
    {
        AbstractScreen::keyPressEvent(event);
    }
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
                           tr("RESTORE ALL SYSTEM PARAMETERS TO FACTORY DEFAULTS?"),
                           this);

    if (confirm.exec() == QDialog::Accepted)
    {
        ConfigManager::getInstance().resetToDefaults();

        m_isDirty = true;

        setupUI();
        update();
    }
}

void SettingsScreen::onBackClicked()
{
    if (m_isDirty)
    {
        TacticalDialog confirm(tr("UNSAVED DATA"),
                               tr("SYSTEM CONFIGURATION HAS BEEN MODIFIED. DISCARD CHANGES?"),
                               this);

        if (confirm.exec() == QDialog::Rejected)
            return;

        ConfigManager::getInstance().loadConfiguration();
    }

    m_isDirty = false;
    MenuManager::getInstance().popScreen();
}

void SettingsScreen::onBindButtonClicked(const QString &actionName)
{
    KeyCaptureDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        Engine::Input::KeyCode newKey = dialog.getCapturedKey();
    }
}

void SettingsScreen::setupUI()
{
    auto *rootLayout = new QHBoxLayout(this);

    rootLayout->addStretch(Config::UI::SETTINGS_SIDE_STRETCH);

    auto *contentContainer = new QWidget();
    contentContainer->setObjectName("settingsContent");
    rootLayout->addWidget(contentContainer, Config::UI::SETTINGS_CONTENT_STRETCH);

    rootLayout->addStretch(Config::UI::SETTINGS_SIDE_STRETCH);

    auto *mainLayout = new QVBoxLayout(contentContainer);

    m_headerLabel = new QLabel(this);
    m_headerLabel->setObjectName("settingsTitle");
    mainLayout->addWidget(m_headerLabel);

    m_tabs = new QTabWidget(this);
    m_tabs->setObjectName("settingsTabs");
    m_tabs->addTab(createGameTab(), "");
    m_tabs->addTab(createInputTab(), "");
    m_tabs->addTab(createDisplayTab(), "");
    m_tabs->addTab(createGraphicsTab(), "");
    m_tabs->addTab(createAudioTab(), "");
    mainLayout->addWidget(m_tabs);

    QHBoxLayout *btnLayout = new QHBoxLayout();

    QPushButton *resetBtn = new QPushButton(tr("RESET TO DEFAULTS"));
    resetBtn->setObjectName("resetButton");
    connect(resetBtn, &QPushButton::clicked, this, &SettingsScreen::onResetClicked);
    btnLayout->addWidget(resetBtn);

    btnLayout->addStretch();

    QPushButton *cancelBtn = new QPushButton(tr("BACK"));
    cancelBtn->setObjectName("cancelButton");

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

QWidget *SettingsScreen::createGameTab()
{
    auto *w = new QWidget();
    auto *layout = new QFormLayout(w);
    auto &cfg = ConfigManager::getInstance().getSettings();

    auto *langCombo = new QComboBox();
    for (const auto &lang : Config::System::LANGUAGES)
    {
        langCombo->addItem(lang.first, lang.second);
    }
    langCombo->setCurrentIndex(cfg.languageIndex);

    auto *tooltipsCheck = new QCheckBox(tr("Enable Context Tooltips"));
    tooltipsCheck->setChecked(cfg.showTooltips);

    layout->addRow(tr("SYSTEM LANGUAGE"), langCombo);
    layout->addRow(tr("USER INTERFACE"), tooltipsCheck);

    connect(langCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, &cfg, langCombo](int index)
            {
        markDirty();
        cfg.languageIndex = index;
        QString langCode = langCombo->itemData(index).toString();
        GameSettingsManager::getInstance().setLanguage(langCode); });

    connect(tooltipsCheck, &QCheckBox::toggled, this, [this, &cfg](bool checked)
            {
        markDirty();
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

    auto *infoLabel = new QLabel(tr("Resolution is locked to native in Borderless mode."), w);

    infoLabel->setObjectName("resolutionInfoLabel");
    infoLabel->setVisible(cfg.windowModeIndex == 1);

    layout->addRow(tr("RESOLUTION"), resCombo);
    layout->addRow("", infoLabel);
    layout->addRow(tr("DISPLAY MODE"), winCombo);
    layout->addRow(tr("VERTICAL SYNC"), vsyncCheck);

    auto updateAvailability = [resCombo, infoLabel](int modeIndex)
    {
        bool isBorderless = (modeIndex == 1);
        resCombo->setEnabled(!isBorderless);
        infoLabel->setVisible(isBorderless);
    };

    connect(resCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, &cfg](int idx)
            {
        markDirty(); 
        cfg.resolutionIndex = idx; });

    connect(winCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, &cfg, updateAvailability](int idx)
            {
        markDirty(); 
        cfg.windowModeIndex = idx;
        updateAvailability(idx); });

    connect(vsyncCheck, &QCheckBox::toggled, this, [this, &cfg](bool chk)
            {
        markDirty(); 
        cfg.vsync = chk; });

    updateAvailability(cfg.windowModeIndex);
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
    gammaSlider->setRange(Config::Gameplay::GAMMA_MIN, Config::Gameplay::GAMMA_MAX);
    gammaSlider->setValue(cfg.gamma);

    layout->addRow(tr("TEXTURE QUALITY"), texCombo);
    layout->addRow(tr("GAMMA CORRECTION"), gammaSlider);

    connect(texCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, &cfg](int index)
            {
        markDirty();
        cfg.textureQualityIndex = index; });

    connect(gammaSlider, &QSlider::valueChanged, this, [this, &cfg](int val)
            {
        markDirty();
        cfg.gamma = val;
        GraphicsSettingsManager::getInstance().applyGraphicsSettings(); });

    return w;
}

QWidget *SettingsScreen::createAudioTab()
{
    auto *w = new QWidget();
    auto *layout = new QFormLayout(w);
    auto &cfg = ConfigManager::getInstance().getSettings();

    struct AudioRow
    {
        QString label;
        int *valuePtr;
    };
    QList<AudioRow> rows = {
        {tr("MASTER VOLUME"), &cfg.masterVol},
        {tr("MUSIC VOLUME"), &cfg.musicVol},
        {tr("SFX VOLUME"), &cfg.sfxVol},
        {tr("VOICE VOLUME"), &cfg.voiceVol}};

    for (const auto &row : rows)
    {
        auto *slider = new QSlider(Qt::Horizontal);
        slider->setRange(Config::Gameplay::VOLUME_MIN, Config::Gameplay::VOLUME_MAX);
        slider->setValue(*row.valuePtr);

        connect(slider, &QSlider::valueChanged, this, [this, row](int val)
                {
            markDirty();
            *row.valuePtr = val;
            emit AudioSettingsManager::getInstance().volumesChanged(); });
        layout->addRow(row.label, slider);
    }
    return w;
}

QWidget *SettingsScreen::createInputTab()
{
    auto *w = new QWidget();
    auto *layout = new QFormLayout(w);

    layout->addRow(new QLabel(tr("KEY BINDINGS")));

    auto createBindRow = [this, layout](const QString &label, Engine::Input::Action action)
    {
        auto *btn = new QPushButton(ControlsSettingsManager::getInstance().getKeyName(action));
        btn->setObjectName("bindButton");

        connect(btn, &QPushButton::clicked, this, [this, btn, action]()
                {
            KeyCaptureDialog diag(this);
            if (diag.exec() == QDialog::Accepted) {
                markDirty(); 
                ControlsSettingsManager::getInstance().setKey(action, static_cast<Engine::Input::KeyCode>(diag.getCapturedKey()));
                btn->setText(ControlsSettingsManager::getInstance().getKeyName(action));
            } });
        layout->addRow(label, btn);
    };

    createBindRow(tr("MOVE UP"), Engine::Input::Action::MOVE_UP);
    createBindRow(tr("MOVE DOWN"), Engine::Input::Action::MOVE_DOWN);
    createBindRow(tr("UNIT: STOP"), Engine::Input::Action::STOP);
    createBindRow(tr("UNIT: GUARD"), Engine::Input::Action::GUARD);
    createBindRow(tr("UNIT: SCATTER"), Engine::Input::Action::SCATTER);

    return w;
}

void SettingsScreen::markDirty()
{
    m_isDirty = true;
}
