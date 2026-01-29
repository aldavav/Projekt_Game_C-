#include "NewGameScreen.h"

NewGameScreen::NewGameScreen(QWidget *parent) : AbstractScreen(parent)
{
    setupUI();
}

void NewGameScreen::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        onLaunchClicked();
    }
    else if (event->key() == Qt::Key_Escape)
    {
        onBackClicked();
    }
    else
    {
        AbstractScreen::keyPressEvent(event);
    }
}

void NewGameScreen::onLaunchClicked()
{
    QString mapName = m_mapNameEdit->text().trimmed();
    if (mapName.isEmpty())
        return;

    QString saveRoot = QCoreApplication::applicationDirPath() + Config::Paths::SAVE_DIR_NAME;
    QDir dir(saveRoot + "/" + mapName);

    if (dir.exists())
    {
        m_mapNameEdit->setStyleSheet("border: 1px solid red; background: #2a0000;");
        return;
    }

    uint seed = m_seedEdit->text().toUInt();
    int difficulty = m_difficultyCombo->currentIndex();
    int mapType = m_mapTypeCombo->currentIndex();

    auto *loading = new LoadingScreen();
    MenuManager::getInstance().setScreen(loading);

    QTimer::singleShot(Config::UI::GENERATION_DELAY, loading, [loading, mapName, seed, difficulty, mapType]()
                       {
        loading->setStatus(tr("GENERATING TERRAIN PROTOCOLS..."));
        loading->setProgress(30);
        
        GameEngine::getInstance().setupMatch(mapName, seed, difficulty, mapType);

        QTimer::singleShot(Config::UI::LOADING_STEP_DELAY, loading, [loading]() {
            loading->setStatus(tr("ESTABLISHING NEURAL LINK..."));
            loading->setProgress(70);
            
            GameEngine::getInstance().startGame();

            QTimer::singleShot(Config::UI::LOADING_STEP_DELAY, loading, [loading]() {
                MenuManager::getInstance().setScreen(new GameScreen());
            });
        }); });
}

void NewGameScreen::onBackClicked()
{
    MenuManager::getInstance().popScreen();
}

void NewGameScreen::setupUI()
{
    auto *rootLayout = new QHBoxLayout(this);

    rootLayout->addStretch(Config::UI::SIDE_STRETCH);

    auto *contentContainer = new QWidget();
    contentContainer->setObjectName("newGameContent");
    rootLayout->addWidget(contentContainer, Config::UI::CONTENT_STRETCH);

    rootLayout->addStretch(Config::UI::SIDE_STRETCH);

    auto *layout = new QVBoxLayout(contentContainer);
    auto *form = new QFormLayout();

    QLabel *header = new QLabel(tr("MISSION DEPLOYMENT"), this);
    header->setObjectName("settingsTitle");
    layout->addWidget(header);

    m_mapNameEdit = new QLineEdit(tr("Default_mission"));
    m_mapNameEdit->setObjectName("missionInput");
    m_mapNameEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("^[a-zA-Z0-9_]*$"), this));
    form->addRow(tr("MAP IDENTIFIER:"), m_mapNameEdit);

    QHBoxLayout *seedLayout = new QHBoxLayout();
    m_seedEdit = new QLineEdit(QString::number(QRandomGenerator::global()->generate()));
    m_seedEdit->setObjectName("missionInput");

    QPushButton *rngBtn = new QPushButton(tr("RNG"));
    rngBtn->setObjectName("secondaryButton");

    connect(rngBtn, &QPushButton::clicked, this, [this]()
            { m_seedEdit->setText(QString::number(QRandomGenerator::global()->generate())); });

    seedLayout->addWidget(m_seedEdit);
    seedLayout->addWidget(rngBtn);
    form->addRow(tr("TERRAIN SEED:"), seedLayout);

    m_difficultyCombo = new QComboBox();
    m_difficultyCombo->addItems({tr("EASY"), tr("NORMAL"), tr("HARD"), tr("BRUTAL")});
    form->addRow(tr("COMBAT DIFFICULTY:"), m_difficultyCombo);

    m_mapTypeCombo = new QComboBox();
    m_mapTypeCombo->addItems({tr("SINGLE ISLAND"), tr("ARCHIPELAGO"), tr("CONTINENTS"), tr("PANGEA")});
    m_mapTypeCombo->setObjectName("missionInput");
    form->addRow(tr("WORLD TOPOLOGY:"), m_mapTypeCombo);

    layout->addLayout(form);

    QPushButton *cancelBtn = new QPushButton(tr("BACK"));
    cancelBtn->setObjectName("cancelButton");

    QPushButton *launchBtn = new QPushButton(tr("INITIALIZE DEPLOYMENT"));
    launchBtn->setObjectName("applyButton");

    connect(cancelBtn, &QPushButton::clicked, this, &NewGameScreen::onBackClicked);
    connect(launchBtn, &QPushButton::clicked, this, &NewGameScreen::onLaunchClicked);

    layout->addStretch();
    layout->addWidget(cancelBtn);
    layout->addWidget(launchBtn);
    m_mapNameEdit->setFocus();
}
