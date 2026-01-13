#include "KeyCaptureDialog.h"

KeyCaptureDialog::KeyCaptureDialog(QWidget *parent) : QDialog(parent)
{
    setModal(true);

    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setFixedSize(300, 150);
    setObjectName("captureDialog");

    auto *layout = new QVBoxLayout(this);
    auto *msg = new QLabel("AWAITING INPUT...\nPRESS ANY KEY TO BIND", this);
    msg->setAlignment(Qt::AlignCenter);

    msg->setStyleSheet("font-weight: bold; color: white;");

    layout->addWidget(msg);
}

void KeyCaptureDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        reject();
    }
    else
    {
        capturedKey = static_cast<Input::KeyCode>(event->key());
        accept();
    }
}
