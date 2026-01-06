#include "UnitBuyDialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

UnitBuyDialog::UnitBuyDialog(const QString& unitName, QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Buy Unit");
    setModal(true);
    setFixedSize(250, 120);

    setStyleSheet(R"(
        QDialog { background-color: #222; }
        QLabel { color: #00ff00; }
        QPushButton {
            background: #444;
            color: white;
            padding: 6px;
        }
    )");
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* label = new QLabel("Buy " + unitName + " ?");
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    QHBoxLayout* buttons = new QHBoxLayout();
    QPushButton* buy = new QPushButton("Buy",this);
    QPushButton* cancel = new QPushButton("Cancel",this);

    buttons->addWidget(buy);
    buttons->addWidget(cancel);
    layout->addLayout(buttons);

    connect(buy, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancel, &QPushButton::clicked, this, &QDialog::reject);
}

