#include "HelpScreen.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QScrollArea>

#include "MenuManager.h"

HelpScreen::HelpScreen(QWidget *parent)
    : AbstractScreen(parent)
{
    setMinimumSize(800, 600);

    setObjectName("HelpRoot");
    setStyleSheet(
        "QWidget#HelpRoot {"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "              stop:0 #05070a, stop:1 #000000);"
        "  color: #D7D7D7;"
        "  font-family: 'Yu Gothic UI';"
        "}"
        "QLabel { background: transparent; border: none; color: #D7D7D7; }"
        );

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(28, 24, 28, 24);
    root->setSpacing(18);

    // Center panel
    auto *panel = new QFrame(this);
    panel->setObjectName("HelpPanel");
    panel->setStyleSheet(
        "QFrame#HelpPanel {"
        "  background-color: rgba(18, 20, 24, 210);"
        "  border: 1px solid rgba(255, 215, 0, 90);"
        "  border-radius: 14px;"
        "}"
        );

    auto *shadow = new QGraphicsDropShadowEffect(panel);
    shadow->setBlurRadius(24);
    shadow->setOffset(0, 10);
    shadow->setColor(QColor(0, 0, 0, 160));
    panel->setGraphicsEffect(shadow);

    root->addWidget(panel);

    auto *main = new QVBoxLayout(panel);
    main->setContentsMargins(28, 22, 28, 22);
    main->setSpacing(14);

    // Title
    auto *title = new QLabel(tr("HELP"), panel);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "QLabel {"
        "  color: #FFD34D;"
        "  font-size: 42px;"
        "  font-weight: 800;"
        "  letter-spacing: 2px;"
        "  padding: 2px 0;"
        "}"
        );
    main->addWidget(title);

    // Subtitle
    auto *info = new QLabel(tr("Quick guide for controls & gameplay basics"), panel);
    info->setAlignment(Qt::AlignHCenter);
    info->setStyleSheet(
        "QLabel {"
        "  color: rgba(215,215,215,200);"
        "  font-size: 15px;"
        "  padding-bottom: 4px;"
        "}"
        );
    main->addWidget(info);

    // Divider
    auto *line = new QFrame(panel);
    line->setFixedHeight(2);
    line->setStyleSheet(
        "QFrame {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "              stop:0 rgba(255,215,0,0),"
        "              stop:0.5 rgba(255,215,0,110),"
        "              stop:1 rgba(255,215,0,0));"
        "  border-radius: 1px;"
        "}"
        );
    main->addWidget(line);

    // ===== Scrollable content =====
    auto *scroll = new QScrollArea(panel);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setStyleSheet(
        "QScrollArea { background: transparent; }"
        "QScrollBar:vertical { width: 10px; background: transparent; }"
        "QScrollBar::handle:vertical {"
        "  background: rgba(255,255,255,50);"
        "  border-radius: 5px;"
        "  min-height: 30px;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: transparent; }"
        );

    auto *content = new QWidget(scroll);
    content->setObjectName("HelpContent");
    content->setStyleSheet("QWidget#HelpContent { background: transparent; }");

    auto *contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(2, 10, 2, 10);
    contentLayout->setSpacing(14);
    contentLayout->setAlignment(Qt::AlignTop);

    scroll->setWidget(content);
    main->addWidget(scroll, 1); // <--- nejdůležitější: dá scrollu expanzní prostor

    auto makeSectionHeader = [&](const QString &text) -> QLabel* {
        auto *hdr = new QLabel(text, content);
        hdr->setStyleSheet(
            "QLabel {"
            "  color: #FFFFFF;"
            "  font-size: 20px;"
            "  font-weight: 800;"
            "  margin-top: 4px;"
            "}"
            );
        return hdr;
    };

    auto makeCard = [&]() -> QFrame* {
        auto *card = new QFrame(content);
        card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        card->setStyleSheet(
            "QFrame {"
            "  background-color: rgba(255,255,255, 7);"
            "  border: 1px solid rgba(255,255,255, 22);"
            "  border-radius: 12px;"
            "}"
            );
        return card;
    };

    auto makeCardText = [&](QFrame *card, const QString &text) {
        auto *lay = new QVBoxLayout(card);
        lay->setContentsMargins(18, 14, 18, 14);
        lay->setSpacing(6);

        auto *lbl = new QLabel(text, card);
        lbl->setWordWrap(true);
        lbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        lbl->setStyleSheet(
            "QLabel {"
            "  font-size: 15px;"
            "  color: rgba(230,230,230,210);"
            "  line-height: 1.25;"
            "}"
            );
        lay->addWidget(lbl);
    };

    // Controls
    contentLayout->addWidget(makeSectionHeader(tr("Controls")));
    {
        auto *card = makeCard();
        makeCardText(card,
                     tr("• <placeholder>\n"
                        "• <placeholder>\n"
                        "• <placeholder>\n"
                        "• <placeholder>\n"
                        "• <placeholder>")
                     );
        contentLayout->addWidget(card);
    }

    // Gameplay
    contentLayout->addWidget(makeSectionHeader(tr("Gameplay")));
    {
        auto *card = makeCard();
        makeCardText(card,
                     tr("• <placeholder>\n"
                        "• <placeholder>\n"
                        "• <placeholder>")
                     );
        contentLayout->addWidget(card);
    }

    // Tips
    contentLayout->addWidget(makeSectionHeader(tr("Tips")));
    {
        auto *card = makeCard();
        makeCardText(card,
                     tr("• <placeholder>\n"
                        "• <placeholder>\n"
                        "• <placeholder>")
                     );
        contentLayout->addWidget(card);
    }

    contentLayout->addStretch(1);

    // ===== Back button fixed at bottom =====
    auto *back = new QPushButton(tr("BACK"), panel);
    back->setCursor(Qt::PointingHandCursor);
    back->setFixedHeight(46);
    back->setMaximumWidth(260);
    back->setStyleSheet(
        "QPushButton {"
        "  font-size: 16px;"
        "  font-weight: 800;"
        "  letter-spacing: 1px;"
        "  padding: 10px 22px;"
        "  border-radius: 10px;"
        "  color: #101010;"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "              stop:0 #FFD34D, stop:1 #CFA21B);"
        "  border: 1px solid rgba(0,0,0,120);"
        "}"
        "QPushButton:hover {"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "              stop:0 #FFE07A, stop:1 #D8AE2B);"
        "}"
        "QPushButton:pressed {"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "              stop:0 #CFA21B, stop:1 #A97F12);"
        "}"
        );

    QObject::connect(back, &QPushButton::clicked, this, []() {
        MenuManager::getInstance().popScreen();
    });

    main->addWidget(back, 0, Qt::AlignHCenter);
}

void HelpScreen::onEnter() {}
void HelpScreen::onExit() {}
