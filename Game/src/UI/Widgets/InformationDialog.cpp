#include "InformationDialog.h"

InformationDialog::InformationDialog(const QString &header, const QString &body, QWidget *parent)
    : QDialog(parent)
{
    setModal(true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(650, 550);

    setupUI(header, body);
}

void InformationDialog::setupUI(const QString &header, const QString &body)
{
    auto *mainLayout = new QVBoxLayout(this);
    QFrame *backgroundFrame = new QFrame(this);
    backgroundFrame->setObjectName("infoBackgroundFrame");
    auto *frameLayout = new QVBoxLayout(backgroundFrame);

    QLabel *titleLabel = new QLabel(header.toUpper());
    titleLabel->setObjectName("infoTitle");
    frameLayout->addWidget(titleLabel);

    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setObjectName("infoScrollArea");
    m_scrollArea->setWidgetResizable(true);

    QLabel *contentLabel = new QLabel(body);
    contentLabel->setObjectName("infoBody");
    contentLabel->setWordWrap(true);

    contentLabel->setTextFormat(Qt::RichText);

    contentLabel->setContentsMargins(10, 10, 15, 10);
    contentLabel->setOpenExternalLinks(true);

    m_scrollArea->setWidget(contentLabel);

    frameLayout->addWidget(m_scrollArea);

    m_acceptCheck = new QCheckBox(tr("I HAVE READ AND AGREE TO THE PROTOCOLS ABOVE"));
    m_acceptCheck->setObjectName("infoCheckBox");
    m_acceptCheck->setEnabled(false);
    frameLayout->addWidget(m_acceptCheck);

    auto *btnLayout = new QHBoxLayout();
    QPushButton *declineBtn = new QPushButton(tr("TERMINATE"));
    declineBtn->setObjectName("infoDeclineBtn");

    m_confirmBtn = new QPushButton(tr("PROCEED"));
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
}

void InformationDialog::handleScroll(int value)
{
    QScrollBar *bar = m_scrollArea->verticalScrollBar();
    if (value >= (bar->maximum() * 0.95))
    {
        m_scrolledToBottom = true;
        m_acceptCheck->setEnabled(true);
    }
}

void InformationDialog::checkRequirements()
{
    m_confirmBtn->setEnabled(m_scrolledToBottom && m_acceptCheck->isChecked());
}

void InformationDialog::onDecline()
{
    QMetaObject::invokeMethod(qApp, "quit", Qt::QueuedConnection);
}

void InformationDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Down || event->key() == Qt::Key_PageDown)
    {
        m_scrollArea->verticalScrollBar()->setValue(m_scrollArea->verticalScrollBar()->value() + 20);
        return;
    }
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        if (m_confirmBtn->isEnabled())
            accept();
    }
    else if (event->key() == Qt::Key_Escape)
    {
        onDecline();
    }
    else
    {
        QDialog::keyPressEvent(event);
    }
}
