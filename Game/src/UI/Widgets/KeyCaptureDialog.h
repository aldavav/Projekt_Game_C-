#ifndef KEYCAPTUREDIALOG_H
#define KEYCAPTUREDIALOG_H

#include <Core/Config/Configuration.h>
#include <Core/Common/GameTypes.h>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QDialog>
#include <QLabel>

class KeyCaptureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KeyCaptureDialog(QWidget *parent = nullptr);

    Engine::Input::KeyCode getCapturedKey() const { return capturedKey; }

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Engine::Input::KeyCode capturedKey = Engine::Input::KeyCode::ESC;
};

#endif
