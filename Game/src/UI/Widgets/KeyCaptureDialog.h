#ifndef KEYCAPTUREDIALOG_H
#define KEYCAPTUREDIALOG_H

#include <Core/Common/KeyCodes.h>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QDialog>
#include <QLabel>

class KeyCaptureDialog : public QDialog
{
    Q_OBJECT
public:
    Input::KeyCode capturedKey = Input::KeyCode::ESC;

    explicit KeyCaptureDialog(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif
