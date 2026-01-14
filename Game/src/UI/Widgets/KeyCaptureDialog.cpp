#include "KeyCaptureDialog.h"

KeyCaptureDialog::KeyCaptureDialog(QWidget *parent) : QDialog(parent)
{
    setModal(true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setFixedSize(400, 200);
    setObjectName("captureDialog");

    auto *layout = new QVBoxLayout(this);

    auto *msg = new QLabel(tr("AWAITING INPUT...\nPRESS ANY KEY TO BIND"), this);
    msg->setAlignment(Qt::AlignCenter);

    layout->addWidget(msg);

    setFocusPolicy(Qt::StrongFocus);
}

void KeyCaptureDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control ||
        event->key() == Qt::Key_Shift ||
        event->key() == Qt::Key_Alt)
    {
        return;
    }

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
