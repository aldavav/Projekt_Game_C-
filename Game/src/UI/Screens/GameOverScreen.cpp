#include "GameOverScreen.h"

GameOverScreen::GameOverScreen(bool victory, QWidget *parent)
    : AbstractScreen(parent)
{
    setObjectName("gameOverScreen");
    setupUI(victory);
}

void GameOverScreen::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape ||
        event->key() == Qt::Key_Return ||
        event->key() == Qt::Key_Enter)
    {
        GameEngine::getInstance().setState(Engine::GameState::Menu);
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

void GameOverScreen::setupUI(bool victory)
{
    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(Config::UI::END_SCREEN_SPACING);

    m_titleLabel = new QLabel(this);

    m_titleLabel->setText(victory ? tr("VICTORY ACHIEVED")
                                  : tr("DEFEAT: SIGNAL LOST"));

    m_titleLabel->setObjectName(victory ? "victoryTitle" : "defeatTitle");
    m_titleLabel->setAlignment(Qt::AlignCenter);

    m_statsLabel = new QLabel(this);
    m_statsLabel->setObjectName("statsBox");

    QString statsText = QString("%1\n\n%2 %3\n%4 %5\n%6 %7")
                            .arg(tr("OPERATIONAL DATA:"))
                            .arg(tr("UNITS PRODUCED:"))
                            .arg(24)
                            .arg(tr("STRUCTURES LOST:"))
                            .arg(2)
                            .arg(tr("TOTAL TIME:"))
                            .arg("18:45");

    m_statsLabel->setText(statsText);
    m_statsLabel->setAlignment(Qt::AlignCenter);

    QPushButton *exitBtn = new QPushButton(tr("RETURN TO HQ"), this);
    exitBtn->setObjectName("primaryButton");
    exitBtn->setFixedSize(Config::UI::END_SCREEN_BTN_WIDTH, Config::UI::END_SCREEN_BTN_HEIGHT);

    exitBtn->setDefault(true);

    connect(exitBtn, &QPushButton::clicked, this, []()
            { GameEngine::getInstance().setState(Engine::GameState::Menu); });

    layout->addStretch();
    layout->addWidget(m_titleLabel);
    layout->addWidget(m_statsLabel);
    layout->addStretch();
    layout->addWidget(exitBtn, 0, Qt::AlignCenter);
    layout->addStretch();
}
