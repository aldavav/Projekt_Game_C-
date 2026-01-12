#include "MainMenuScreen.h"

namespace
{
    constexpr int kTitleHeight = 120;

    constexpr int kSocketWidth = 450;
    constexpr int kSocketHeight = 80;

    constexpr int kNormalWidth = 250;
    constexpr int kNormalHeight = 70;

    constexpr int kHoverWidth = 265;
    constexpr int kHoverHeight = 55;

    const QString kStandardButtonStyle = R"(
        QPushButton {
            font-size: 24px;
            border-image: url(:/images/assets/images/button.png) 0 0 0 0 stretch stretch;
            border-width: 0px;
            padding-bottom: 8px; 
            color: white;
        }
        QPushButton:hover {
            background-color: rgba(230, 138, 0, 220);
            border-radius: 20px;
        }
    )";

    const QString kQuitButtonStyle = R"(
    QPushButton {
        font-size: 24px;
        font-weight: bold;
        border-radius: 10px;
        color: #FFFFFF;
        background-color: #700000; 
        border: 2px solid #FF3131;
    }
    QPushButton:hover { 
        background-color: #B20000; 
        border: 2px solid #FF9191; 
        color: #FFFAFA;
    }
    QPushButton:pressed {
        background-color: #330000;
        border: 2px solid #990000;
        color: #888888;
    }
)";

    constexpr int kArrowSize = 100;

    class HoverScaleFilter : public QObject
    {
    public:
        HoverScaleFilter(QPushButton *btn, QLabel *arrow, QObject *parent = nullptr)
            : QObject(parent), m_btn(btn), m_arrow(arrow) {}

        bool eventFilter(QObject *obj, QEvent *event) override
        {
            if (event->type() == QEvent::Enter)
            {
                m_btn->setFixedSize(kHoverWidth, kHoverHeight);

                int newX = (kSocketWidth - kHoverWidth) / 2;
                int newY = (kSocketHeight - kHoverHeight) / 2;
                m_btn->move(newX, newY);

                m_arrow->show();
                if (auto *glow = m_btn->graphicsEffect())
                    glow->setEnabled(true);
            }
            else if (event->type() == QEvent::Leave)
            {

                m_btn->setFixedSize(kNormalWidth, kNormalHeight);

                int newX = (kSocketWidth - kNormalWidth) / 2;
                int newY = (kSocketHeight - kNormalHeight) / 2;
                m_btn->move(newX, newY);

                m_arrow->hide();
                if (auto *glow = m_btn->graphicsEffect())
                    glow->setEnabled(false);
            }
            return false;
        }

    private:
        QPushButton *m_btn;
        QLabel *m_arrow;
    };

    void applyEffects(QPushButton *button, QLabel *arrow)
    {
        auto *glow = new QGraphicsDropShadowEffect(button);
        glow->setBlurRadius(25);
        glow->setOffset(0, 0);
        glow->setColor(QColor(255, 200, 80, 180));
        glow->setEnabled(false);
        button->setGraphicsEffect(glow);

        button->installEventFilter(new HoverScaleFilter(button, arrow, button));
    }
}

MainMenuScreen::MainMenuScreen(QWidget *parent)
    : AbstractScreen(parent)
{
    setObjectName("menu");

    m_backgroundLabel = new QLabel(this);
    m_backgroundLabel->setScaledContents(true);
    m_bgMovie = new QMovie(":/animations/assets/animations/menu.gif");
    m_bgMovie->setSpeed(50);
    m_bgMovie->setCacheMode(QMovie::CacheAll);
    m_backgroundLabel->setMovie(m_bgMovie);

    connect(m_bgMovie, &QMovie::frameChanged, this, [this](int frameNumber)
            {
        if (frameNumber == m_bgMovie->frameCount() - 1) {
            m_bgMovie->stop();
        } });

    m_backgroundLabel->lower();
    m_bgMovie->start();

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mainLayout->addSpacing(20);

    auto *titleLabel = new QLabel(tr("COMMAND & CONQUER"), this);
    titleLabel->setMinimumHeight(kTitleHeight);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFont(QFont("Yu Gothic UI", 48, QFont::Bold));
    titleLabel->setStyleSheet("background: transparent; color: #FFD700;");
    mainLayout->addWidget(titleLabel);

    mainLayout->addSpacing(40);

    auto *gridLayout = new QGridLayout();
    gridLayout->setVerticalSpacing(5);

    struct MenuEntry
    {
        QString text;
        QString style;
        void (MainMenuScreen::*slot)();
    };

    const MenuEntry entries[] = {
        {tr("NEW GAME"), kStandardButtonStyle, &MainMenuScreen::onNewGameClicked},
        {tr("LOAD GAME"), kStandardButtonStyle, &MainMenuScreen::onLoadGameClicked},
        {tr("SETTINGS"), kStandardButtonStyle, &MainMenuScreen::onSettingsClicked},
        {tr("HELP"), kStandardButtonStyle, &MainMenuScreen::onHelpClicked},
        {tr("CREDITS"), kStandardButtonStyle, &MainMenuScreen::onCreditsClicked},
        {tr("QUIT"), kQuitButtonStyle, &MainMenuScreen::onQuitClicked}};

    for (int row = 0; row < (int)std::size(entries); ++row)
    {
        const auto &e = entries[row];

        QWidget *socket = new QWidget(this);
        socket->setFixedSize(kSocketWidth, kSocketHeight);

        QLabel *arrow = new QLabel(socket);
        arrow->setPixmap(QPixmap(":/images/assets/images/arrow.png").scaled(kArrowSize - 15, kArrowSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        arrow->setFixedSize(kArrowSize, kArrowSize);
        arrow->hide();

        auto *btn = new QPushButton(e.text, socket);
        btn->setFixedSize(kNormalWidth, kNormalHeight);
        btn->setStyleSheet(e.style);

        int btnX = (kSocketWidth - kNormalWidth) / 2;
        int btnY = (kSocketHeight - kNormalHeight) / 2;
        btn->move(btnX, btnY);

        int arrowX = btnX + kNormalWidth + 15;
        int arrowY = (kSocketHeight - kArrowSize) / 2;
        arrow->move(arrowX, arrowY);

        connect(btn, &QPushButton::clicked, this, e.slot);

        if (e.text == tr("QUIT"))
        {
            auto *neonGlow = new QGraphicsDropShadowEffect(btn);
            neonGlow->setBlurRadius(20);
            neonGlow->setOffset(0, 0);
            neonGlow->setColor(QColor(255, 49, 49, 200));
            btn->setGraphicsEffect(neonGlow);
        }
        else
        {
            applyEffects(btn, arrow);
        }

        gridLayout->addWidget(socket, row, 0, Qt::AlignCenter);
    }

    m_sidePanel = new QWidget(this);
    m_sidePanel->setFixedWidth(380);
    m_sidePanel->setFixedHeight(300);
    m_sidePanel->setStyleSheet("background-color: rgba(0, 20, 0, 230); border: 2px solid #FFD700; border-radius: 5px;");
    m_sidePanel->hide();

    auto *panelLayout = new QVBoxLayout(m_sidePanel);
    panelLayout->setContentsMargins(15, 10, 15, 15);

    auto *headerLayout = new QHBoxLayout();

    m_panelTitle = new QLabel("INFORMATION", m_sidePanel);
    m_panelTitle->setFont(QFont("Yu Gothic UI", 14, QFont::Bold));
    m_panelTitle->setStyleSheet("color: #FFD700; border: none;");

    QPushButton *closeBtn = new QPushButton("X", m_sidePanel);
    closeBtn->setFixedSize(24, 24);
    closeBtn->setStyleSheet("QPushButton { background: #440000; color: white; border: 1px solid #FFD700; font-weight: bold; } QPushButton:hover { background: #aa0000; }");
    connect(closeBtn, &QPushButton::clicked, m_sidePanel, &QWidget::hide);

    headerLayout->addWidget(m_panelTitle);
    headerLayout->addStretch();
    headerLayout->addWidget(closeBtn);
    panelLayout->addLayout(headerLayout);

    QFrame *line = new QFrame(m_sidePanel);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background-color: rgba(255, 215, 0, 100);");
    panelLayout->addWidget(line);

    m_panelText = new QLabel("", m_sidePanel);
    m_panelText->setStyleSheet("color: #00FF00; border: none; font-family: 'Consolas'; font-size: 13px;");
    m_panelText->setWordWrap(true);
    m_panelText->setAlignment(Qt::AlignTop);
    panelLayout->addWidget(m_panelText);
    panelLayout->addStretch();

    auto *hLayout = new QHBoxLayout();
    hLayout->addStretch(1);
    hLayout->addLayout(gridLayout);
    hLayout->addSpacing(50);
    hLayout->addWidget(m_sidePanel, 0, Qt::AlignVCenter);
    hLayout->addStretch(3);

    mainLayout->addLayout(hLayout);
    mainLayout->addStretch();
}

void MainMenuScreen::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (m_backgroundLabel)
    {
        m_backgroundLabel->setGeometry(0, 0, width(), height());
    }
}

void MainMenuScreen::onEnter() { LOG_INFO("Entering MainMenuScreen."); }

void MainMenuScreen::onExit()
{
    LOG_INFO("Exiting MainMenuScreen.");

    if (m_bgMovie)
    {
        m_bgMovie->stop();
    }

    this->hide();
}

void MainMenuScreen::onNewGameClicked()
{
    LOG_INFO("New Game clicked, preparing transition...");

    this->setEnabled(false);

    if (m_bgMovie)
        m_bgMovie->stop();

    GameEngine::getInstance().startGame();

    MenuManager::getInstance().setScreen(new GameScreen());
}

void MainMenuScreen::onLoadGameClicked() { LOG_INFO("Load Game clicked."); }
void MainMenuScreen::onSettingsClicked() { LOG_INFO("Settings clicked."); }

void MainMenuScreen::onHelpClicked()
{
    m_panelTitle->setText("SYSTEM MANUAL");
    m_panelText->setText(
        "CONTROLS:\n"
        " - Left Click: Select unit/hex\n"
        " - Right Click Drag: Pan Camera\n"
        " - Scroll: Zoom In/Out\n\n"
        "TIME CONTROLS:\n"
        " - [1][2][3] or HUD: Change Speed\n"
        " - [Space]: Pause Simulation\n\n"
        "GOAL: Reveal the map and discover resources.");

    if (!m_sidePanel->isVisible())
        m_sidePanel->show();
    LOG_INFO("Help Manual Accessed.");
}

void MainMenuScreen::onCreditsClicked()
{
    m_panelTitle->setText("PERSONNEL DOSSIER");
    m_panelText->setText(
        "CHIEF ARCHITECT: [Your Name]\n"
        "ENGINE: Gemini 3 Flash\n"
        "GRAPHICS: Nano Banana\n\n"
        "STATUS: ONLINE\n"
        "STRIKE TEAM: C++ / Qt Framework");

    if (!m_sidePanel->isVisible())
        m_sidePanel->show();
    LOG_INFO("Credits Dossier Accessed.");
}

void MainMenuScreen::onQuitClicked() { QCoreApplication::quit(); }
