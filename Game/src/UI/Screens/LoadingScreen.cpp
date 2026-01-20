#include "LoadingScreen.h"

LoadingScreen::LoadingScreen(QWidget *parent) : AbstractScreen(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    QLabel *title = new QLabel(tr(Config::DEFAULT_UPLINK_TEXT.toUtf8()), this);
    title->setObjectName("loadingTitle");
    title->setAlignment(Qt::AlignCenter);

    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);

    m_progressBar->setFixedSize(Config::LOADING_BAR_WIDTH, Config::LOADING_BAR_HEIGHT);
    m_progressBar->setTextVisible(false);

    m_statusLabel = new QLabel(tr(Config::DEFAULT_STATUS_TEXT.toUtf8()), this);
    m_statusLabel->setObjectName("statusLabel");
    m_statusLabel->setAlignment(Qt::AlignCenter);

    layout->addStretch();
    layout->addWidget(title);
    layout->addWidget(m_progressBar);
    layout->addWidget(m_statusLabel);
    layout->addStretch();
}

void LoadingScreen::onEnter() { this->show(); }

void LoadingScreen::onExit() { this->hide(); }

void LoadingScreen::setProgress(int value)
{
    m_progressBar->setValue(value);
    m_progressBar->repaint();
}

void LoadingScreen::setStatus(const QString &status)
{
    m_statusLabel->setText(status);
}

void LoadingScreen::resizeEvent(QResizeEvent *event)
{
    AbstractScreen::resizeEvent(event);
    update();
}
