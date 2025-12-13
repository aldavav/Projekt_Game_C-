#include "MainMenuScreen.h"

MainMenuScreen::MainMenuScreen(QWidget *parent)
    : AbstractScreen(parent)
{

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    this->setStyleSheet("QWidget { background-color: #000000; }");

    QLabel *titleLabel = new QLabel(tr("Command & Conquer"), this);
    QFont titleFont("Yu Gothic UI", 48, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setMinimumSize(0, 120);
    titleLabel->setStyleSheet("color: #FFD700; /* Gold/Yellow for C&C Title */");
    titleLabel->setAlignment(Qt::AlignCenter);

    mainLayout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed));

    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setSpacing(10);

    gridLayout->setContentsMargins(0, 0, 500, 0);

    QString standardStyle = R"(
        QPushButton {
            font-size: 24px;
            padding: 10px 30px;
            border-radius: 10px;
            color: white; 
            background: #337AB7; 
        }
        QPushButton:hover, QPushButton:focus {
            background-color: #E68A00;
            border: 2px solid white;
            color: #FFFF99;
        }
        QPushButton:pressed {
            background-color: #CC7A00;
        }
        QPushButton:disabled {
            background-color: #666666;
            color: #AAAAAA;
            border: 1px solid #444444;
        }
    )";

    QString quitStyle = R"(
        QPushButton {
            font-size: 24px;
            padding: 10px 30px;
            border-radius: 10px;
            color: white; 
            background: #C9302C; 
        }
        QPushButton:hover, QPushButton:focus {
            background-color: #A92C28;
            border: 2px solid white;
            color: #FF4500; /* Orange/Red for hover */
        }
        QPushButton:pressed {
            background-color: #902622;
        }
        QPushButton:disabled {
            background-color: #666666;
            color: #AAAAAA;
            border: 1px solid #444444;
        }
    )";

    QSize buttonSize(250, 50);

    QPushButton *buttonNewGame = new QPushButton(tr("NEW GAME"), this);
    buttonNewGame->setMinimumSize(buttonSize);
    buttonNewGame->setMaximumSize(350, buttonSize.height());
    buttonNewGame->setStyleSheet(standardStyle);
    connect(buttonNewGame, &QPushButton::clicked, this, &MainMenuScreen::onNewGameClicked);
    gridLayout->addWidget(buttonNewGame, 0, 0);

    QPushButton *buttonLoadGame = new QPushButton(tr("LOAD GAME"), this);
    buttonLoadGame->setMinimumSize(buttonSize);
    buttonLoadGame->setMaximumSize(350, buttonSize.height());
    buttonLoadGame->setStyleSheet(standardStyle);
    connect(buttonLoadGame, &QPushButton::clicked, this, &MainMenuScreen::onLoadGameClicked);
    gridLayout->addWidget(buttonLoadGame, 1, 0);

    QPushButton *buttonSettings = new QPushButton(tr("SETTINGS"), this);
    buttonSettings->setMinimumSize(buttonSize);
    buttonSettings->setMaximumSize(350, buttonSize.height());
    buttonSettings->setStyleSheet(standardStyle);
    connect(buttonSettings, &QPushButton::clicked, this, &MainMenuScreen::onSettingsClicked);
    gridLayout->addWidget(buttonSettings, 2, 0);

    QPushButton *buttonHelp = new QPushButton(tr("HELP"), this);
    buttonHelp->setMinimumSize(buttonSize);
    buttonHelp->setMaximumSize(350, buttonSize.height());
    buttonHelp->setStyleSheet(standardStyle);
    connect(buttonHelp, &QPushButton::clicked, this, &MainMenuScreen::onHelpClicked);
    gridLayout->addWidget(buttonHelp, 3, 0);

    QPushButton *buttonCredits = new QPushButton(tr("CREDITS"), this);
    buttonCredits->setMinimumSize(buttonSize);
    buttonCredits->setMaximumSize(350, buttonSize.height());
    buttonCredits->setStyleSheet(standardStyle);
    connect(buttonCredits, &QPushButton::clicked, this, &MainMenuScreen::onCreditsClicked);
    gridLayout->addWidget(buttonCredits, 4, 0);

    QPushButton *buttonQuit = new QPushButton(tr("QUIT"), this);
    buttonQuit->setMinimumSize(buttonSize);
    buttonQuit->setMaximumSize(350, buttonSize.height());
    buttonQuit->setStyleSheet(quitStyle);
    connect(buttonQuit, &QPushButton::clicked, this, &MainMenuScreen::onQuitClicked);
    gridLayout->addWidget(buttonQuit, 5, 0);

    mainLayout->addLayout(gridLayout);

    mainLayout->addStretch(1);
}

MainMenuScreen::~MainMenuScreen()
{
}

void MainMenuScreen::onEnter()
{
    LOG_INFO("Entering MainMenuScreen.");
}

void MainMenuScreen::onExit()
{
    LOG_INFO("Exiting MainMenuScreen.");
}

void MainMenuScreen::onNewGameClicked()
{
    LOG_INFO("New Game clicked. Starting game.");

    Map::getInstance().initializeNewMap("Default Campaign", Difficulty::EASY);

    MenuManager::getInstance().setScreen(new GameScreen(parentWidget()));
}

void MainMenuScreen::onLoadGameClicked()
{
    LOG_INFO("Load Game clicked. (Functionality pending implementation).");
}

void MainMenuScreen::onSettingsClicked()
{
    LOG_INFO("Settings clicked. (Functionality pending implementation).");
}

void MainMenuScreen::onHelpClicked()
{
    LOG_INFO("Help clicked. (Functionality pending implementation).");
}

void MainMenuScreen::onCreditsClicked()
{
    LOG_INFO("Credits clicked. (Functionality pending implementation).");
}

void MainMenuScreen::onQuitClicked()
{
    LOG_INFO("Quit Game clicked.");
    QCoreApplication::quit();
}
