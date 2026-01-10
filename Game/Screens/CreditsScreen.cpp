#include "CreditsScreen.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QFont>
#include <QGraphicsDropShadowEffect>

CreditsScreen::CreditsScreen(QWidget *parent)
    : AbstractScreen(parent)
{
    setMinimumSize(800, 600);

    // Root styling (background gradient + default text)
    setObjectName("CreditsRoot");
    setStyleSheet(
        "QWidget#CreditsRoot {"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "              stop:0 #05070a, stop:1 #000000);"
        "  color: #D7D7D7;"
        "  font-family: 'Yu Gothic UI';"
        "}"
        "QLabel { color: #D7D7D7; }"
        );

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(28, 24, 28, 24);
    root->setSpacing(18);
    root->setAlignment(Qt::AlignTop);

    // Center panel (card)
    auto *panel = new QFrame(this);
    panel->setObjectName("CreditsPanel");
    panel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    panel->setStyleSheet(
        "QFrame#CreditsPanel {"
        "  background-color: rgba(18, 20, 24, 210);"
        "  border: 1px solid rgba(255, 215, 0, 90);"
        "  border-radius: 14px;"
        "}"
        );

    // Subtle shadow to lift the panel
    auto *shadow = new QGraphicsDropShadowEffect(panel);
    shadow->setBlurRadius(24);
    shadow->setOffset(0, 10);
    shadow->setColor(QColor(0, 0, 0, 160));
    panel->setGraphicsEffect(shadow);

    root->addWidget(panel);

    auto *mainLayout = new QVBoxLayout(panel);
    mainLayout->setContentsMargins(28, 22, 28, 22);
    mainLayout->setSpacing(16);
    mainLayout->setAlignment(Qt::AlignTop);

    // Title
    auto *title = new QLabel(tr("CREDITS"), panel);
    QFont titleFont("Yu Gothic UI", 42, QFont::Bold);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "QLabel {"
        "  color: #FFD34D;"
        "  letter-spacing: 2px;"
        "  padding: 6px 0;"
        "}"
        );
    mainLayout->addWidget(title);

    // Small subtitle
    auto *info = new QLabel(
        tr("Command & Conquer – Student Project\nBuilt with C++ and Qt Widgets"),
        panel
        );
    info->setAlignment(Qt::AlignHCenter);
    info->setStyleSheet(
        "QLabel {"
        "  color: rgba(215, 215, 215, 200);"
        "  font-size: 15px;"
        "  line-height: 1.25;"
        "}"
        );
    mainLayout->addWidget(info);

    // Divider (thin + glow-ish)
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
    mainLayout->addWidget(line);

    // Helper lambda to create section blocks
    auto makeSection = [&](const QString &headerText) -> QLabel* {
        auto *hdr = new QLabel(headerText, panel);
        QFont hdrFont("Yu Gothic UI", 18, QFont::Bold);
        hdr->setFont(hdrFont);
        hdr->setStyleSheet(
            "QLabel {"
            "  color: #FFFFFF;"
            "  padding-top: 6px;"
            "}"
            );
        return hdr;
    };

    auto makeCard = [&]() -> QFrame* {
        auto *card = new QFrame(panel);
        card->setStyleSheet(
            "QFrame {"
            "  background-color: rgba(255,255,255, 6);"
            "  border: 1px solid rgba(255,255,255, 18);"
            "  border-radius: 12px;"
            "}"
            );
        return card;
    };

    // TEAM section
    mainLayout->addWidget(makeSection(tr("Team")));

    auto *teamCard = makeCard();
    auto *teamLayout = new QVBoxLayout(teamCard);
    teamLayout->setContentsMargins(16, 14, 16, 14);
    teamLayout->setSpacing(8);

    auto *team = new QLabel(
        tr("Vavřinec Aleš, Szotkowski Michael, Hovorka Marek"),
        teamCard
        );
    team->setStyleSheet(
        "QLabel {"
        "  font-size: 16px;"
        "  color: rgba(235,235,235, 230);"
        "}"
        );
    teamLayout->addWidget(team);

    mainLayout->addWidget(teamCard);

    // NOTES section
    mainLayout->addWidget(makeSection(tr("Notes")));

    auto *notesCard = makeCard();
    auto *notesLayout = new QVBoxLayout(notesCard);
    notesLayout->setContentsMargins(16, 14, 16, 14);
    notesLayout->setSpacing(8);

    auto *notes = new QLabel(
        tr("Assets and references are used for educational purposes.\n"
           "This project is not affiliated with the original Command & Conquer."),
        notesCard
        );
    notes->setWordWrap(true);
    notes->setStyleSheet(
        "QLabel {"
        "  font-size: 14px;"
        "  color: rgba(210,210,210, 200);"
        "}"
        );
    notesLayout->addWidget(notes);

    mainLayout->addWidget(notesCard);

    mainLayout->addStretch(1);

    // BACK button (center, game-like)
    auto *back = new QPushButton(tr("BACK"), panel);
    back->setCursor(Qt::PointingHandCursor);
    back->setFixedHeight(46);
    back->setMaximumWidth(260);
    back->setStyleSheet(
        "QPushButton {"
        "  font-size: 16px;"
        "  font-weight: 700;"
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
        "QPushButton:disabled {"
        "  color: rgba(16,16,16,120);"
        "  background: rgba(255,255,255,40);"
        "}"
        );

    QObject::connect(back, &QPushButton::clicked, this, []() {
        MenuManager::getInstance().popScreen();
    });

    mainLayout->addWidget(back, 0, Qt::AlignHCenter);
}

void CreditsScreen::onEnter()
{
}

void CreditsScreen::onExit()
{
}
