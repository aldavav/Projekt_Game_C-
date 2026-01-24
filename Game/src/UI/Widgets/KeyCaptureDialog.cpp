#include "KeyCaptureDialog.h"

KeyCaptureDialog::KeyCaptureDialog(QWidget *parent)
    : BaseTacticalDialog(
          QSize(Config::UI::KEY_DIALOG_WIDTH, Config::UI::KEY_DIALOG_HEIGHT),
          "keyCaptureDialog",
          parent)
{
    setupUI();
}

void KeyCaptureDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control || event->key() == Qt::Key_Shift ||
        event->key() == Qt::Key_Alt || event->key() == Qt::Key_Meta)
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

void KeyCaptureDialog::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    int margin = Config::UI::KEY_DIALOG_MARGINS;
    mainLayout->setContentsMargins(margin, margin, margin, margin);

    QFrame *backgroundFrame = new QFrame(this);
    backgroundFrame->setObjectName("captureDialog");
    auto *frameLayout = new QVBoxLayout(backgroundFrame);

    auto *infoLabel = new QLabel(tr("AWAITING INPUT...\n\nPRESS ANY KEY TO BIND\n[ESC] TO CANCEL"), this);
    infoLabel->setObjectName("keyCaptureInfo");
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setWordWrap(true);

    frameLayout->addWidget(infoLabel);
    mainLayout->addWidget(backgroundFrame);
}
