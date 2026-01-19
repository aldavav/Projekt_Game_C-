#include "GameOverScreen.h"

GameOverScreen::GameOverScreen(bool victory, QWidget *parent)
    : AbstractScreen(parent)
{
    setObjectName("gameOverScreen");
    setupUI(victory);
}

void GameOverScreen::onEnter() { this->show(); }

void GameOverScreen::onExit() { this->hide(); }

void GameOverScreen::setupUI(bool victory)
{
    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(Config::END_SCREEN_SPACING);

    m_titleLabel = new QLabel(this);

    m_titleLabel->setText(victory ? tr(Config::TEXT_VICTORY.toUtf8())
                                  : tr(Config::TEXT_DEFEAT.toUtf8()));

    m_titleLabel->setObjectName(victory ? "victoryTitle" : "defeatTitle");
    m_titleLabel->setAlignment(Qt::AlignCenter);

    m_statsLabel = new QLabel(this);
    m_statsLabel->setObjectName("statsBox");

    QString statsText = QString("%1\n\n%2 %3\n%4 %5\n%6 %7")
                            .arg(tr(GameConfig::STATS_HEADER.toUtf8()))
                            .arg(tr(GameConfig::STATS_UNITS.toUtf8()))
                            .arg(24)
                            .arg(tr(GameConfig::STATS_LOSSES.toUtf8()))
                            .arg(2)
                            .arg(tr(GameConfig::STATS_TIME.toUtf8()))
                            .arg("18:45");

    m_statsLabel->setText(statsText);
    m_statsLabel->setAlignment(Qt::AlignCenter);

    QPushButton *exitBtn = new QPushButton(tr("RETURN TO MAIN MENU"), this);
    exitBtn->setObjectName("primaryButton");
    exitBtn->setFixedSize(Config::END_SCREEN_BTN_WIDTH, Config::END_SCREEN_BTN_HEIGHT);

    connect(exitBtn, &QPushButton::clicked, this, []()
            { GameEngine::getInstance().setState(STATE_MENU); });

    layout->addStretch();
    layout->addWidget(m_titleLabel);
    layout->addWidget(m_statsLabel);
    layout->addWidget(exitBtn);
    layout->addStretch();
}
