#ifndef KEYCAPTUREDIALOG_H
#define KEYCAPTUREDIALOG_H

#include <Core/Common/KeyCodes.h>
#include <Core/Config/Config.h>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QDialog>
#include <QLabel>

class KeyCaptureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KeyCaptureDialog(QWidget *parent = nullptr);

    Input::KeyCode getCapturedKey() const { return capturedKey; }

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Input::KeyCode capturedKey = Input::KeyCode::ESC;
};

#endif
