#include "GameOverScreen.h"

GameOverScreen::GameOverScreen(bool victory, QWidget *parent)
    : AbstractScreen(parent)
{
    setObjectName("gameOverScreen");
    setupUI(victory);
}

void GameOverScreen::onEnter()
{
    this->show();
    this->setFocus();
}

void GameOverScreen::onExit() { this->hide(); }

void GameOverScreen::setupUI(bool victory)
{
    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(Config::UI::END_SCREEN_SPACING);

    m_titleLabel = new QLabel(this);

    m_titleLabel->setText(victory ? tr(Config::Gameplay::TEXT_VICTORY.toUtf8())
                                  : tr(Config::Gameplay::TEXT_DEFEAT.toUtf8()));

    m_titleLabel->setObjectName(victory ? "victoryTitle" : "defeatTitle");
    m_titleLabel->setAlignment(Qt::AlignCenter);

    m_statsLabel = new QLabel(this);
    m_statsLabel->setObjectName("statsBox");

    QString statsText = QString("%1\n\n%2 %3\n%4 %5\n%6 %7")
                            .arg(tr(Config::UI::STATS_HEADER.toUtf8()))
                            .arg(tr(Config::UI::STATS_UNITS.toUtf8()))
                            .arg(24)
                            .arg(tr(Config::UI::STATS_LOSSES.toUtf8()))
                            .arg(2)
                            .arg(tr(Config::UI::STATS_TIME.toUtf8()))
                            .arg("18:45");

    m_statsLabel->setText(statsText);
    m_statsLabel->setAlignment(Qt::AlignCenter);

    QPushButton *exitBtn = new QPushButton(tr("RETURN TO HQ"), this);
    exitBtn->setObjectName("primaryButton");
    exitBtn->setFixedSize(Config::UI::END_SCREEN_BTN_WIDTH, Config::UI::END_SCREEN_BTN_HEIGHT);

    exitBtn->setDefault(true);

    connect(exitBtn, &QPushButton::clicked, this, []()
            { GameEngine::getInstance().setState(Engine::State::MENU); });

    layout->addStretch();
    layout->addWidget(m_titleLabel);
    layout->addWidget(m_statsLabel);
    layout->addStretch();
    layout->addWidget(exitBtn, 0, Qt::AlignCenter);
    layout->addStretch();
}

void GameOverScreen::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape ||
        event->key() == Qt::Key_Return ||
        event->key() == Qt::Key_Enter)
    {
        GameEngine::getInstance().setState(Engine::State::MENU);
    }
    else
    {
        AbstractScreen::keyPressEvent(event);
    }
}

void GameOverScreen::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    QColor overlayColor = m_titleLabel->objectName() == "victoryTitle"
                              ? QColor(0, 20, 40, 200)
                              : QColor(40, 0, 0, 200);

    painter.fillRect(rect(), overlayColor);
}
