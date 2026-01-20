#include "TacticalDialog.h"

TacticalDialog::TacticalDialog(const QString &title, const QString &message, QWidget *parent)
    : QDialog(parent)
{
    setModal(true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setObjectName("tacticalDialog");

    setFixedSize(Config::DIALOG_WIDTH, Config::DIALOG_HEIGHT);

    setupUI(title, message);
}

void TacticalDialog::setupUI(const QString &title, const QString &message)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 15, 20, 15);
    layout->setSpacing(10);

    auto *titleLabel = new QLabel(title.toUpper());
    titleLabel->setObjectName("dialogTitle");
    layout->addWidget(titleLabel);

    auto *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setObjectName("panelLine");
    layout->addWidget(line);

    auto *bodyLabel = new QLabel(message);
    bodyLabel->setObjectName("dialogBody");
    bodyLabel->setWordWrap(true);
    bodyLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(bodyLabel);

    layout->addStretch();

    auto *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(15);

    auto *cancelBtn = new QPushButton(Config::STR_ABORT);
    cancelBtn->setObjectName("secondaryButton");

    auto *confirmBtn = new QPushButton(Config::STR_PROCEED);
    confirmBtn->setObjectName("confirmBtn");

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(confirmBtn, &QPushButton::clicked, this, &QDialog::accept);

    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(confirmBtn);
    layout->addLayout(btnLayout);

    cancelBtn->setFocus();
}

void TacticalDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        reject();
    }
    else
    {
        QDialog::keyPressEvent(event);
    }
}
