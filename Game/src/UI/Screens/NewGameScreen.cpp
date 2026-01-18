#include "NewGameScreen.h"

NewGameScreen::NewGameScreen(QWidget *parent) : AbstractScreen(parent)
{
    setupUI();
}

void NewGameScreen::onEnter() { this->show(); }

void NewGameScreen::onExit() { this->hide(); }

void NewGameScreen::setupUI()
{
    auto *rootLayout = new QHBoxLayout(this);

    rootLayout->addStretch(Config::UI_SIDE_STRETCH);

    auto *contentContainer = new QWidget();
    contentContainer->setObjectName("newGameContent");
    rootLayout->addWidget(contentContainer, Config::UI_CONTENT_STRETCH);

    rootLayout->addStretch(Config::UI_SIDE_STRETCH);

    auto *layout = new QVBoxLayout(contentContainer);
    auto *form = new QFormLayout();

    QLabel *header = new QLabel(tr("MISSION DEPLOYMENT"), this);
    header->setObjectName("settingsTitle");
    layout->addWidget(header);

    m_mapNameEdit = new QLineEdit(Config::DEFAULT_MISSION_NAME);
    m_mapNameEdit->setObjectName("missionInput");
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
    m_difficultyCombo->addItems(GameConfig::DIFFICULTIES);
    form->addRow(tr("COMBAT DIFFICULTY:"), m_difficultyCombo);

    m_opponentCombo = new QComboBox();
    m_opponentCombo->addItems(GameConfig::OPPONENTS);
    form->addRow(tr("OPPONENT TYPE:"), m_opponentCombo);

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
}

void NewGameScreen::onLaunchClicked()
{
    QString mapName = m_mapNameEdit->text();
    uint seed = m_seedEdit->text().toUInt();

    auto *loading = new LoadingScreen();
    MenuManager::getInstance().setScreen(loading);

    QTimer::singleShot(Config::GENERATION_DELAY, loading, [loading, mapName, seed]()
                       {
        loading->setStatus(tr("GENERATING TERRAIN..."));
        loading->setProgress(30);
        
        GameEngine::getInstance().setupMatch(mapName, seed);

        QTimer::singleShot(Config::LOADING_STEP_DELAY, loading, [loading]() {
            loading->setStatus(tr("CREATING SECURE ARCHIVE..."));
            loading->setProgress(60);
            
            GameEngine::getInstance().startGame();
            MenuManager::getInstance().updateMetadata();

            QTimer::singleShot(Config::LOADING_FINAL_PAUSE, loading, []() {
                auto* game = new GameScreen();
                MenuManager::getInstance().setScreen(game);
                
                QTimer::singleShot(0, game, [game]() {
                    game->setFocus();
                    game->update(); 
                });
            });
        }); });
}

void NewGameScreen::onBackClicked()
{
    MenuManager::getInstance().popScreen();
}
