#include "InformationDialog.h"

InformationDialog::InformationDialog(const QString &header, const QString &body, QWidget *parent)
    : BaseTacticalDialog(QSize(Config::UI::INFO_DIALOG_WIDTH, Config::UI::INFO_DIALOG_HEIGHT),
                         "informationDialog", parent)
{
    setupUI(header, body);

    QTimer::singleShot(100, this, [this]()
                       {
        QScrollBar *bar = m_scrollArea->verticalScrollBar();
        if (!bar->isVisible() || bar->maximum() <= 0) {
            handleScroll(0); 
        } });
}

void InformationDialog::handleEscape()
{
    onDecline();
}

void InformationDialog::onDecline()
{
    QCoreApplication::quit();
}

void InformationDialog::keyPressEvent(QKeyEvent *event)
{
    QScrollBar *bar = m_scrollArea->verticalScrollBar();

    switch (event->key())
    {
    case Qt::Key_Down:
    case Qt::Key_PageDown:
        bar->setValue(bar->value() + Config::Input::KEYBOARD_SCROLL_STEP);
        break;
    case Qt::Key_Up:
    case Qt::Key_PageUp:
        bar->setValue(bar->value() - Config::Input::KEYBOARD_SCROLL_STEP);
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        if (m_confirmBtn->isEnabled())
            accept();
        break;
    default:
        BaseTacticalDialog::keyPressEvent(event);
    }
}

void InformationDialog::handleScroll(int value)
{
    QScrollBar *bar = m_scrollArea->verticalScrollBar();

    if (!bar->isVisible() || bar->maximum() <= 0 || value >= (bar->maximum() * Config::UI::LEGAL_SCROLL_THRESHOLD))
    {
        m_scrolledToBottom = true;
        m_acceptCheck->setEnabled(true);
    }
}

void InformationDialog::checkRequirements()
{
    m_confirmBtn->setEnabled(m_scrolledToBottom && m_acceptCheck->isChecked());
}

void InformationDialog::setupUI(const QString &header, const QString &body)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QFrame *backgroundFrame = new QFrame(this);
    backgroundFrame->setObjectName("infoBackgroundFrame");
    auto *frameLayout = new QVBoxLayout(backgroundFrame);

    QLabel *titleLabel = new QLabel(header.toUpper(), backgroundFrame);
    titleLabel->setObjectName("infoTitle");
    frameLayout->addWidget(titleLabel);

    m_scrollArea = new QScrollArea(backgroundFrame);
    m_scrollArea->setObjectName("infoScrollArea");
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);

    QLabel *contentLabel = new QLabel(body, m_scrollArea);
    contentLabel->setObjectName("infoBody");
    contentLabel->setWordWrap(true);
    contentLabel->setTextFormat(Qt::RichText);
    contentLabel->setOpenExternalLinks(true);

    m_scrollArea->setWidget(contentLabel);
    frameLayout->addWidget(m_scrollArea);

    m_acceptCheck = new QCheckBox(tr("I HAVE READ AND AGREE TO THE PROTOCOLS ABOVE"), backgroundFrame);
    m_acceptCheck->setObjectName("infoCheckBox");
    m_acceptCheck->setEnabled(false);
    frameLayout->addWidget(m_acceptCheck);

    auto *btnLayout = new QHBoxLayout();
    QPushButton *declineBtn = new QPushButton(tr("TERMINATE"), backgroundFrame);
    declineBtn->setObjectName("infoDeclineBtn");

    m_confirmBtn = new QPushButton(tr("PROCEED"), backgroundFrame);
    m_confirmBtn->setObjectName("infoConfirmBtn");
    m_confirmBtn->setEnabled(false);

    connect(declineBtn, &QPushButton::clicked, this, &InformationDialog::onDecline);
    connect(m_confirmBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_acceptCheck, &QCheckBox::checkStateChanged, this, &InformationDialog::checkRequirements);
    connect(m_scrollArea->verticalScrollBar(), &QScrollBar::valueChanged, this, &InformationDialog::handleScroll);

    btnLayout->addWidget(declineBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(m_confirmBtn);
    frameLayout->addLayout(btnLayout);
    mainLayout->addWidget(backgroundFrame);

    m_scrollArea->setFocus();
}
