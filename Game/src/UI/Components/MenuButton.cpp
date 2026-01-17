#include "MenuButton.h"

MenuButton::MenuButton(const QString &text, bool isQuit, QWidget *parent)
    : QWidget(parent)
{
    m_arrow = new QLabel(this);
    m_arrow->setObjectName("buttonArrow");
    m_arrow->move(350, -10);
    m_arrow->hide();

    m_btn = new QPushButton(text, this);
    m_btn->setObjectName(isQuit ? "quitButton" : "menuButton");

    m_btn->move(100, 5);

    auto *glow = new QGraphicsDropShadowEffect(m_btn);
    glow->setBlurRadius(25);
    glow->setOffset(0, 0);

    glow->setColor(isQuit ? QColor(255, 49, 49) : QColor(255, 200, 80));
    glow->setEnabled(isQuit);
    m_btn->setGraphicsEffect(glow);

    m_btn->installEventFilter(this);
    connect(m_btn, &QPushButton::clicked, this, &MenuButton::clicked);
}

bool MenuButton::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_btn)
    {
        if (event->type() == QEvent::Enter || event->type() == QEvent::FocusIn)
        {
            m_btn->setFixedSize(265, 75);
            m_btn->move(92, 2);
            m_arrow->move(m_btn->x() + m_btn->width() + 10, (height() - m_arrow->height()) / 2);
            m_arrow->show();

            if (auto *glow = m_btn->graphicsEffect())
                glow->setEnabled(true);
        }
        else if (event->type() == QEvent::Leave || event->type() == QEvent::FocusOut)
        {
            m_btn->setFixedSize(250, 70);
            m_btn->move(100, 5);
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
