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
    QString mapName = m_mapNameEdit->text();
    uint seed = m_seedEdit->text().toUInt();

    auto *loading = new LoadingScreen();
    MenuManager::getInstance().setScreen(loading);

    QTimer::singleShot(Config::UI::GENERATION_DELAY, loading, [loading, mapName, seed]()
                       {
        loading->setStatus(tr("GENERATING TERRAIN PROTOCOLS..."));
        loading->setProgress(30);
        
        GameEngine::getInstance().setupMatch(mapName, seed);

        QTimer::singleShot(Config::UI::LOADING_STEP_DELAY, loading, [loading]() {
            loading->setStatus(tr("ESTABLISHING NEURAL LINK..."));
            loading->setProgress(70);
            
            GameEngine::getInstance().startGame();
            MenuManager::getInstance().updateMetadata();

            QTimer::singleShot(Config::UI::LOADING_FINAL_PAUSE, loading, []() {
                auto* game = new GameScreen();
                MenuManager::getInstance().setScreen(game);
                game->setFocus();
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

    m_mapNameEdit = new QLineEdit(Config::Gameplay::DEFAULT_MISSION_NAME);
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
    m_difficultyCombo->addItems(Config::UI::DIFFICULTIES);
    form->addRow(tr("COMBAT DIFFICULTY:"), m_difficultyCombo);

    m_opponentCombo = new QComboBox();
    m_opponentCombo->addItems(Config::UI::OPPONENTS);
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
    m_mapNameEdit->setFocus();
}
