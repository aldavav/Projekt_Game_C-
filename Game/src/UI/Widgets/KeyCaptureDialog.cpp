#include "KeyCaptureDialog.h"

KeyCaptureDialog::KeyCaptureDialog(QWidget *parent)
{
    setModal(true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setObjectName("keyCaptureDialog");
    setFixedSize(300, 150);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);

    auto *infoLabel = new QLabel(tr("Press a key to bind, or ESC to cancel"));
    infoLabel->setObjectName("keyCaptureInfo");
    infoLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(infoLabel);
}

void KeyCaptureDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control || event->key() == Qt::Key_Shift || event->key() == Qt::Key_Alt)
        return;

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
