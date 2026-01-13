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

    QLabel *header = new QLabel("SYSTEM CONFIGURATION", this);
    header->setObjectName("settingsTitle");
    mainLayout->addWidget(header);

    m_tabs = new QTabWidget(this);
    m_tabs->addTab(createGameTab(), "GAME");
    m_tabs->addTab(createInputTab(), "CONTROLS");
    m_tabs->addTab(createDisplayTab(), "DISPLAY");
    m_tabs->addTab(createGraphicsTab(), "GRAPHICS");
    m_tabs->addTab(createAudioTab(), "AUDIO");

    mainLayout->addWidget(m_tabs);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    QPushButton *cancelBtn = new QPushButton("BACK");
    QPushButton *applyBtn = new QPushButton("APPLY CHANGES");
    applyBtn->setObjectName("applyButton");

    connect(cancelBtn, &QPushButton::clicked, this, &SettingsScreen::onBackClicked);
    connect(applyBtn, &QPushButton::clicked, this, &SettingsScreen::onApplyClicked);

    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(applyBtn);
    mainLayout->addLayout(btnLayout);
}

QWidget *SettingsScreen::createGameTab()
{
    auto *w = new QWidget();
    auto *layout = new QFormLayout(w);
    auto &cfg = ConfigManager::getInstance().getSettings();

    auto *langCombo = new QComboBox();
    langCombo->addItem("English");
    langCombo->setCurrentIndex(cfg.languageIndex);

    auto *tooltipsCheck = new QCheckBox("Show Context Tooltips");
    tooltipsCheck->setChecked(cfg.showTooltips);

    layout->addRow("LANGUAGE", langCombo);
    layout->addRow("INTERFACE", tooltipsCheck);

    connect(langCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, &cfg](int index)
            {
        m_isDirty = true;
        cfg.languageIndex = index; });
    connect(tooltipsCheck, &QCheckBox::toggled, this, [this, &cfg](bool checked)
            {
        m_isDirty = true;
        cfg.showTooltips = checked; });

    return w;
}

QWidget *SettingsScreen::createDisplayTab()
{
    auto *w = new QWidget();
    auto *layout = new QFormLayout(w);
    auto &cfg = ConfigManager::getInstance().getSettings();

    auto *resCombo = new QComboBox();
    resCombo->addItems({"1920x1080", "2560x1440", "3840x2160"});
    resCombo->setCurrentIndex(cfg.resolutionIndex);

    auto *winCombo = new QComboBox();
    winCombo->addItems({"Fullscreen", "Borderless Window", "Windowed"});
    winCombo->setCurrentIndex(cfg.windowModeIndex);

    auto *vsyncCheck = new QCheckBox();
    vsyncCheck->setChecked(cfg.vsync);

    layout->addRow("RESOLUTION", resCombo);
    layout->addRow("DISPLAY MODE", winCombo);
    layout->addRow("VERTICAL SYNC", vsyncCheck);

    connect(resCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, &cfg](int index)
            {
        m_isDirty = true;
        cfg.resolutionIndex = index; });
    connect(winCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, &cfg](int index)
            {
        m_isDirty = true;
        cfg.windowModeIndex = index; });
    connect(vsyncCheck, &QCheckBox::toggled, this, [this, &cfg](bool checked)
            {
        m_isDirty = true;
        cfg.vsync = checked; });

    return w;
}

QWidget *SettingsScreen::createGraphicsTab()
{
    auto *w = new QWidget();
    auto *layout = new QFormLayout(w);
    auto &cfg = ConfigManager::getInstance().getSettings();

    auto *texCombo = new QComboBox();
    texCombo->addItems({"Low", "Medium", "High", "Ultra"});
    texCombo->setCurrentIndex(cfg.textureQualityIndex);

    auto *gammaSlider = new QSlider(Qt::Horizontal);
    gammaSlider->setRange(0, 100);
    gammaSlider->setValue(cfg.gamma);

    layout->addRow("TEXTURE QUALITY", texCombo);
    layout->addRow("GAMMA CALIBRATION", gammaSlider);

    connect(texCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, &cfg](int index)
            {
        m_isDirty = true;
        cfg.textureQualityIndex = index; });
    connect(gammaSlider, &QSlider::valueChanged, this, [this, &cfg](int val)
            {
        m_isDirty = true;
        cfg.gamma = val; });

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

    masterSlider->setValue(cfg.masterVol);
    musicSlider->setValue(cfg.musicVol);
    sfxSlider->setValue(cfg.sfxVol);
    voiceSlider->setValue(cfg.voiceVol);

    QMap<QSlider *, int *> sliderMap = {
        {masterSlider, &cfg.masterVol}, {musicSlider, &cfg.musicVol}, {sfxSlider, &cfg.sfxVol}, {voiceSlider, &cfg.voiceVol}};

    for (auto it = sliderMap.begin(); it != sliderMap.end(); ++it)
    {
        it.key()->setRange(0, 100);
        int *targetValue = it.value();
        connect(it.key(), &QSlider::valueChanged, this, [this, targetValue](int val)
                {
            m_isDirty = true;
            *targetValue = val; });
    }

    layout->addRow("MASTER VOLUME", masterSlider);
    layout->addRow("MUSIC VOLUME", musicSlider);
    layout->addRow("SFX VOLUME", sfxSlider);
    layout->addRow("VOICE VOLUME", voiceSlider);

    return w;
}

QWidget *SettingsScreen::createInputTab()
{
    auto *w = new QWidget();
    auto *layout = new QFormLayout(w);

    auto *sensSlider = new QSlider(Qt::Horizontal);
    auto *invertCheck = new QCheckBox();

    layout->addRow("MOUSE SENSITIVITY", sensSlider);
    layout->addRow("INVERT Y-AXIS", invertCheck);

    connect(sensSlider, &QSlider::valueChanged, this, [this]
            { m_isDirty = true; });
    connect(invertCheck, &QCheckBox::toggled, this, [this]
            { m_isDirty = true; });

    layout->addRow(new QLabel("KEY BINDINGS"));

    auto createBindRow = [this, layout](const QString &label, KeyBindingManager::Action action)
    {
        auto *btn = new QPushButton(KeyBindingManager::getInstance().getKeyName(action));
        btn->setObjectName("bindButton");

        connect(btn, &QPushButton::clicked, this, [this, btn, action]()
                {
            KeyCaptureDialog diag(this);
            if (diag.exec() == QDialog::Accepted) {
                m_isDirty = true; 
                KeyBindingManager::getInstance().setKey(action, static_cast<Input::KeyCode>(diag.capturedKey));
                btn->setText(KeyBindingManager::getInstance().getKeyName(action));
            } });

        layout->addRow(label, btn);
    };

    createBindRow("MOVE UP", KeyBindingManager::Action::MOVE_UP);
    createBindRow("MOVE DOWN", KeyBindingManager::Action::MOVE_DOWN);
    createBindRow("UNIT: STOP", KeyBindingManager::Action::STOP);
    createBindRow("UNIT: GUARD", KeyBindingManager::Action::GUARD);
    createBindRow("UNIT: SCATTER", KeyBindingManager::Action::SCATTER);

    return w;
}

void SettingsScreen::onApplyClicked()
{
    ConfigManager::getInstance().saveConfiguration();
    m_isDirty = false;
    MenuManager::getInstance().popScreen();
}

void SettingsScreen::onBackClicked()
{
    if (m_isDirty)
    {
        TacticalDialog confirm(
            "Unsaved Changes",
            "Configuration modified. Discard changes and exit to terminal?",
            this);

        if (confirm.exec() == QDialog::Rejected)
            return;

        ConfigManager::getInstance().loadConfiguration();
    }
    MenuManager::getInstance().popScreen();
}
