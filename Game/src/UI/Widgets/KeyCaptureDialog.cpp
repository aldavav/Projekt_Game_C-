#include "KeyCaptureDialog.h"

KeyCaptureDialog::KeyCaptureDialog(QWidget *parent)
{
    setModal(true);

    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setObjectName("keyCaptureDialog");

    setFixedSize(Config::KEY_DIALOG_WIDTH, Config::KEY_DIALOG_HEIGHT);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(Config::KEY_DIALOG_MARGINS,
                               Config::KEY_DIALOG_MARGINS,
                               Config::KEY_DIALOG_MARGINS,
                               Config::KEY_DIALOG_MARGINS);

    auto *infoLabel = new QLabel(tr("Press a key to bind, or ESC to cancel"));
    infoLabel->setObjectName("keyCaptureInfo");
    infoLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(infoLabel);
}

void KeyCaptureDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control ||
        event->key() == Qt::Key_Shift ||
        event->key() == Qt::Key_Alt ||
        event->key() == Qt::Key_Meta)
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
