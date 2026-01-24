#include "TacticalDialog.h"

TacticalDialog::TacticalDialog(const QString &title, const QString &message, QWidget *parent)
    : BaseTacticalDialog(QSize(Config::UI::DIALOG_WIDTH, Config::UI::DIALOG_HEIGHT),
                         "tacticalDialog",
                         parent)
{
    setupUI(title, message);
}

void TacticalDialog::setupUI(const QString &title, const QString &message)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 15, 20, 15);
    mainLayout->setSpacing(10);

    QFrame *backgroundFrame = new QFrame(this);
    backgroundFrame->setObjectName("tacticalDialogContainer");
    auto *frameLayout = new QVBoxLayout(backgroundFrame);

    auto *titleLabel = new QLabel(title.toUpper(), this);
    titleLabel->setObjectName("dialogTitle");
    frameLayout->addWidget(titleLabel);

    auto *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setObjectName("panelLine");
    frameLayout->addWidget(line);

    auto *bodyLabel = new QLabel(message, this);
    bodyLabel->setObjectName("dialogBody");
    bodyLabel->setWordWrap(true);
    bodyLabel->setAlignment(Qt::AlignCenter);
    frameLayout->addWidget(bodyLabel);

    frameLayout->addStretch();

    auto *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(15);

    auto *cancelBtn = new QPushButton(Config::UI::STR_ABORT, this);
    cancelBtn->setObjectName("secondaryButton");

    auto *confirmBtn = new QPushButton(Config::UI::STR_PROCEED, this);
    confirmBtn->setObjectName("confirmBtn");

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(confirmBtn, &QPushButton::clicked, this, &QDialog::accept);

    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(confirmBtn);
    frameLayout->addLayout(btnLayout);
    mainLayout->addWidget(backgroundFrame);

    cancelBtn->setFocus();
}
