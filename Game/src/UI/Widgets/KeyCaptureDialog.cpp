#include "KeyCaptureDialog.h"

KeyCaptureDialog::KeyCaptureDialog(QWidget *parent)
    : QDialog(parent)
{
    setModal(true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setObjectName("keyCaptureDialog");

    setFixedSize(Config::UI::KEY_DIALOG_WIDTH, Config::UI::KEY_DIALOG_HEIGHT);

    auto *layout = new QVBoxLayout(this);
    int margin = Config::UI::KEY_DIALOG_MARGINS;
    layout->setContentsMargins(margin, margin, margin, margin);

    auto *infoLabel = new QLabel(tr("AWAITING INPUT...\n\nPRESS ANY KEY TO BIND\n[ESC] TO CANCEL"));
    infoLabel->setObjectName("keyCaptureInfo");
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setWordWrap(true);

    layout->addWidget(infoLabel);
}

void KeyCaptureDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control ||
        event->key() == Qt::Key_Shift ||
        event->key() == Qt::Key_Alt ||
        event->key() == Qt::Key_Meta)
    {
        return;
    }
    if (event->key() == Qt::Key_Escape)
    {
        reject();
    }
    else
    {
        capturedKey = static_cast<Engine::Input::KeyCode>(event->key());
        accept();
    }
}
