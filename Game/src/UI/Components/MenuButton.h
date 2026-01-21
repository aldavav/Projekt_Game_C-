#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <Core/Config/Configuration.h>
#include <QGraphicsDropShadowEffect>
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QEvent>

class QPushButton;
class QLabel;

class MenuButton : public QWidget
{
    Q_OBJECT
public:
    explicit MenuButton(const QString &text, bool isQuit = false, QWidget *parent = nullptr);

signals:
    void clicked();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QPushButton *m_btn;

    QLabel *m_arrow;
};

#endif
