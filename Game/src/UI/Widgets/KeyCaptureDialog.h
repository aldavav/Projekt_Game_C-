#ifndef KEYCAPTUREDIALOG_H
#define KEYCAPTUREDIALOG_H

#include <UI/Widgets/BaseTacticalDialog.h>
#include <Core/Config/Configuration.h>
#include <QVBoxLayout>
#include <QLabel>

class KeyCaptureDialog : public BaseTacticalDialog
{
    Q_OBJECT

public:
    explicit KeyCaptureDialog(QWidget *parent = nullptr);

    Engine::Input::KeyCode getCapturedKey() const { return capturedKey; }

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void setupUI();

    Engine::Input::KeyCode capturedKey = Engine::Input::KeyCode::ESC;
};

#endif
