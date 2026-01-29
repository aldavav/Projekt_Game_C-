#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <Core/Config/Configuration.h>
#include <QGraphicsDropShadowEffect>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
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

    void setText(const QString &text);

signals:
    void clicked();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void setupUI(const QString &header, bool isQuit);

    void setupAnimations();

    QPushButton *m_btn = nullptr;

    QLabel *m_arrow = nullptr;

    QParallelAnimationGroup *m_hoverAnimation = nullptr;

    QPropertyAnimation *m_moveAnim = nullptr;

    QPropertyAnimation *m_sizeAnim = nullptr;
};

#endif
