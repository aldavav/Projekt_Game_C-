#include "MenuButton.h"

MenuButton::MenuButton(const QString &text, bool isQuit, QWidget *parent)
    : QWidget(parent)
{
    setupUI(text, isQuit);
    setupAnimations();
}

void MenuButton::setupAnimations()
{
    m_hoverAnimation = new QParallelAnimationGroup(this);

    m_moveAnim = new QPropertyAnimation(m_btn, "pos", this);
    m_moveAnim->setDuration(150);
    m_moveAnim->setEasingCurve(QEasingCurve::OutCubic);

    m_sizeAnim = new QPropertyAnimation(m_btn, "size", this);
    m_sizeAnim->setDuration(150);
    m_sizeAnim->setEasingCurve(QEasingCurve::OutCubic);

    m_hoverAnimation->addAnimation(m_moveAnim);
    m_hoverAnimation->addAnimation(m_sizeAnim);
}

bool MenuButton::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_btn)
    {
        if (event->type() == QEvent::Enter || event->type() == QEvent::FocusIn)
        {
            m_hoverAnimation->stop();

            m_moveAnim->setStartValue(m_btn->pos());
            m_moveAnim->setEndValue(QPoint(Config::UI::BTN_X_HOVER, 2));

            m_sizeAnim->setStartValue(m_btn->size());
            m_sizeAnim->setEndValue(QSize(Config::UI::BTN_WIDTH_HOVER, Config::UI::BTN_HEIGHT_HOVER));

            m_hoverAnimation->setDirection(QAbstractAnimation::Forward);
            m_hoverAnimation->start();

            m_arrow->move(m_btn->x() + m_btn->width() + 10, (height() - m_arrow->height()) / 2);
            m_arrow->show();

            if (auto *glow = m_btn->graphicsEffect())
                glow->setEnabled(true);
        }
        else if (event->type() == QEvent::Leave || event->type() == QEvent::FocusOut)
        {
            m_hoverAnimation->stop();

            m_moveAnim->setStartValue(m_btn->pos());
            m_moveAnim->setEndValue(QPoint(Config::UI::BTN_X_NORMAL, 5));

            m_sizeAnim->setStartValue(m_btn->size());
            m_sizeAnim->setEndValue(QSize(Config::UI::BTN_WIDTH_NORMAL, Config::UI::BTN_HEIGHT_NORMAL));

            m_hoverAnimation->setDirection(QAbstractAnimation::Forward);
            m_hoverAnimation->start();

            m_arrow->hide();

            if (m_btn->objectName() != "quitButton")
            {
                if (auto *glow = m_btn->graphicsEffect())
                    glow->setEnabled(false);
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

void MenuButton::setupUI(const QString &text, bool isQuit)
{
    m_arrow = new QLabel(this);
    m_arrow->setPixmap(QPixmap(Config::Paths::ARROW_ICON));
    m_arrow->setObjectName("buttonArrow");
    m_arrow->hide();

    m_btn = new QPushButton(text, this);
    m_btn->setStyleSheet(QString("QPushButton#menuButton { border-image: url(%1) 0 0 0 0 stretch stretch; }").arg(Config::Paths::BUTTON_BG));
    m_btn->setObjectName(isQuit ? "quitButton" : "menuButton");
    m_btn->setFixedSize(Config::UI::BTN_WIDTH_NORMAL, Config::UI::BTN_HEIGHT_NORMAL);
    m_btn->move(Config::UI::BTN_X_NORMAL, 5);

    auto *glow = new QGraphicsDropShadowEffect(m_btn);
    glow->setBlurRadius(25);
    glow->setOffset(0, 0);
    glow->setEnabled(isQuit);
    m_btn->setGraphicsEffect(glow);

    m_btn->installEventFilter(this);
    connect(m_btn, &QPushButton::clicked, this, &MenuButton::clicked);
}
