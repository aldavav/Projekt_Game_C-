#include "GameOverScreen.h"

GameOverScreen::GameOverScreen(bool victory, QWidget *parent)
    : AbstractScreen(parent)
{
    setObjectName("gameOverScreen");
    setupUI(victory);
}

void GameOverScreen::setupUI(bool victory)
{
    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(30);

    m_titleLabel = new QLabel(this);
    m_titleLabel->setText(victory ? tr("VICTORY ACHIEVED") : tr("DEFEAT: SIGNAL LOST"));
    m_titleLabel->setObjectName(victory ? "victoryTitle" : "defeatTitle");
    m_titleLabel->setAlignment(Qt::AlignCenter);

    m_statsLabel = new QLabel(this);
    m_statsLabel->setObjectName("statsBox");

    m_statsLabel->setText(tr("OPERATIONAL DATA:\n\n"
                             "UNITS PRODUCED: 24\n"
                             "STRUCTURES LOST: 2\n"
                             "TOTAL TIME: 18:45"));
    m_statsLabel->setAlignment(Qt::AlignCenter);

    QPushButton *exitBtn = new QPushButton(tr("RETURN TO MAIN MENU"), this);
    exitBtn->setObjectName("primaryButton");
    exitBtn->setFixedSize(300, 50);

    connect(exitBtn, &QPushButton::clicked, this, []()
            { GameEngine::getInstance().setState(STATE_MENU); });

    layout->addStretch();
    layout->addWidget(m_titleLabel);
    layout->addWidget(m_statsLabel);
    layout->addWidget(exitBtn);
    layout->addStretch();
}

void GameOverScreen::onEnter() { this->show(); }
void GameOverScreen::onExit() { this->hide(); }
