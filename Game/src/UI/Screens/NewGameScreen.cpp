#include "NewGameScreen.h"

NewGameScreen::NewGameScreen(QWidget *parent) : AbstractScreen(parent)
{
    setupUI();
}

void NewGameScreen::onEnter() { this->show(); }

void NewGameScreen::onExit() { this->hide(); }

void NewGameScreen::setupUI()
{
    auto *layout = new QVBoxLayout(this);
    auto *form = new QFormLayout();
    layout->setContentsMargins(100, 50, 100, 50);

    QLabel *header = new QLabel(tr("MISSION DEPLOYMENT"), this);
    header->setObjectName("settingsTitle");
    layout->addWidget(header);

    m_mapNameEdit = new QLineEdit("Sector_7");
    form->addRow(tr("MAP IDENTIFIER:"), m_mapNameEdit);

    QHBoxLayout *seedLayout = new QHBoxLayout();
    m_seedEdit = new QLineEdit(QString::number(QRandomGenerator::global()->generate()));
    QPushButton *rngBtn = new QPushButton(tr("RNG"));
    connect(rngBtn, &QPushButton::clicked, this, [this]()
            { m_seedEdit->setText(QString::number(QRandomGenerator::global()->generate())); });
    seedLayout->addWidget(m_seedEdit);
    seedLayout->addWidget(rngBtn);
    form->addRow(tr("TERRAIN SEED:"), seedLayout);

    m_difficultyCombo = new QComboBox();
    m_difficultyCombo->addItems({tr("EASY"), tr("NORMAL"), tr("HARD"), tr("BRUTAL")});
    form->addRow(tr("COMBAT DIFFICULTY:"), m_difficultyCombo);

    m_opponentCombo = new QComboBox();
    m_opponentCombo->addItems({tr("AI: NEURAL NETWORK"), tr("HUMAN: UPLINK (NETWORK)")});
    form->addRow(tr("OPPONENT TYPE:"), m_opponentCombo);

    layout->addLayout(form);

    QPushButton *cancelBtn = new QPushButton(tr("BACK"));
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

    QTimer::singleShot(500, loading, [loading, mapName, seed]()
                       {
        loading->setStatus(tr("GENERATING TERRAIN..."));
        loading->setProgress(30);
        
        
        GameEngine::getInstance().setupMatch(mapName, seed);

        QTimer::singleShot(100, loading, [loading]() {
            loading->setStatus(tr("CREATING SECURE ARCHIVE..."));
            loading->setProgress(60);
            
            GameEngine::getInstance().startGame();

            QTimer::singleShot(500, loading, []() {
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
