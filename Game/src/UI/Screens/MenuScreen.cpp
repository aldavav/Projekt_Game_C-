#include "MenuScreen.h"

MenuScreen::MenuScreen(QWidget *parent)
    : AbstractScreen(parent)
{
    setupBackground();
    setupUI();
}

void MenuScreen::onEnter()
{
    if (m_bgMovie)
        m_bgMovie->start();
    this->show();
}

void MenuScreen::onExit()
{
    if (m_bgMovie)
        m_bgMovie->stop();
    this->hide();
}

void MenuScreen::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (m_backgroundLabel)
    {
        m_backgroundLabel->setGeometry(0, 0, width(), height());
    }
}

void MenuScreen::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    if (m_buttonLayout->count() > 0)
    {
        if (auto *firstBtn = qobject_cast<QWidget *>(m_buttonLayout->itemAt(0)->widget()))
        {
            firstBtn->setFocus();
        }
    }
}

void MenuScreen::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Up:
        focusNextPrevChild(false);
        break;

    case Qt::Key_Down:
        focusNextPrevChild(true);
        break;

    case Qt::Key_Enter:
    case Qt::Key_Return:
    {
        auto *focused = qApp->focusWidget();

        if (auto *btn = qobject_cast<QPushButton *>(focused))
        {
            btn->click();
        }
        break;
    }

    case Qt::Key_Escape:
        if (m_sidePanel && m_sidePanel->isVisible())
        {
            m_sidePanel->hide();
        }
        else
        {
            onQuitClicked();
        }
        break;

    default:
        AbstractScreen::keyPressEvent(event);
    }
}

void MenuScreen::onNewGameClicked()
{
    MenuManager::getInstance().pushScreen(new NewGameScreen(this->parentWidget()));
}

void MenuScreen::onLoadGameClicked()
{
    MenuManager::getInstance().pushScreen(new LoadGameScreen(this->parentWidget()));
}

void MenuScreen::onSettingsClicked()
{
    MenuManager::getInstance().pushScreen(new SettingsScreen());
}

void MenuScreen::onHelpClicked()
{
    updatePanelContent(Config::UI::MANUAL_TITLE, Config::UI::MANUAL_TEXT);
}

void MenuScreen::onCreditsClicked()
{
    QString content = "LEAD PROGRAMMER\nLEAD GAME DESIGNER\nTECHNICAL ARCHITECT\n"
                      "CORE SYSTEMS & GAMEPLAY\n- " +
                      Config::Strings::LEAD_DEV + "\n\n";

    content += "PROGRAMMING SUPPORT\n- " + Config::Strings::PROGRAMMING_TEAM.join("\n- ") + "\n\n";

    content += "ADDITIONAL CONTRIBUTIONS\n- " + Config::Strings::CONTRIBUTIONS.join("\n- ") + "\n\n";

    content += "ENGINE & FRAMEWORK:\nQt / C++\n\n"
               "ASSETS:\nAssets by Pixelrepo (https://pixelrepo.com)\n\n"
               "TESTING:\nDeveloped and tested by the team";

    updatePanelContent("PERSONNEL DOSSIER", content);
}

void MenuScreen::onQuitClicked()
{
    if (window())
    {
        window()->close();
    }
}

void MenuScreen::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mainLayout->setContentsMargins(Config::UI::MENU_LEFT_MARGIN, 20, 40, 40);

    auto *titleLabel = new QLabel(Config::System::GAME_TITLE, this);
    titleLabel->setObjectName("mainMenuTitle");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    mainLayout->addSpacing(30);

    auto *contentLayout = new QHBoxLayout();
    auto *buttonContainer = new QWidget(this);
    m_buttonLayout = new QVBoxLayout(buttonContainer);
    m_buttonLayout->setSpacing(5);

    struct MenuEntry
    {
        QString text;
        void (MenuScreen::*slot)();
        bool isQuit;
    };

    const MenuEntry entries[] = {
        {tr("NEW GAME"), &MenuScreen::onNewGameClicked, false},
        {tr("LOAD GAME"), &MenuScreen::onLoadGameClicked, false},
        {tr("SETTINGS"), &MenuScreen::onSettingsClicked, false},
        {tr("HELP"), &MenuScreen::onHelpClicked, false},
        {tr("CREDITS"), &MenuScreen::onCreditsClicked, false},
        {tr("QUIT"), &MenuScreen::onQuitClicked, true}};

    for (const auto &e : entries)
    {
        auto *btn = new MenuButton(e.text, e.isQuit, this);
        connect(btn, &MenuButton::clicked, this, e.slot);
        m_buttonLayout->addWidget(btn);
    }

    contentLayout->addWidget(buttonContainer);
    contentLayout->addStretch(1);

    m_sidePanel = new QWidget(this);
    m_sidePanel->setObjectName("sidePanel");
    m_sidePanel->hide();

    auto *panelLayout = new QVBoxLayout(m_sidePanel);
    panelLayout->setContentsMargins(15, 10, 15, 15);

    auto *headerLayout = new QHBoxLayout();
    m_panelTitle = new QLabel("INFORMATION", m_sidePanel);
    m_panelTitle->setObjectName("panelTitle");

    auto *closeBtn = new QPushButton("X", m_sidePanel);
    closeBtn->setObjectName("panelCloseBtn");
    closeBtn->setFixedSize(24, 24);
    connect(closeBtn, &QPushButton::clicked, m_sidePanel, &QWidget::hide);

    headerLayout->addWidget(m_panelTitle);
    headerLayout->addStretch();
    headerLayout->addWidget(closeBtn);
    panelLayout->addLayout(headerLayout);

    auto *line = new QFrame(m_sidePanel);
    line->setObjectName("panelLine");
    line->setFrameShape(QFrame::HLine);
    panelLayout->addWidget(line);

    m_panelText = new QLabel("", m_sidePanel);
    m_panelText->setObjectName("panelDescription");
    m_panelText->setWordWrap(true);
    m_panelText->setAlignment(Qt::AlignTop);
    panelLayout->addWidget(m_panelText);
    panelLayout->addStretch();

    contentLayout->addWidget(m_sidePanel, 0, Qt::AlignVCenter);
    contentLayout->addStretch(2);

    mainLayout->addLayout(contentLayout);
    mainLayout->addStretch();

    auto *versionLabel = new QLabel("v" + Config::System::VERSION, this);
    versionLabel->setObjectName("versionLabel");

    auto *footerLayout = new QHBoxLayout();
    footerLayout->addStretch();
    footerLayout->addWidget(versionLabel);
    mainLayout->addLayout(footerLayout);
    
    if (m_buttonLayout->count() > 0)
    {
        if (auto *firstBtn = qobject_cast<QWidget *>(m_buttonLayout->itemAt(0)->widget()))
        {
            firstBtn->setFocus();
        }
    }
}

void MenuScreen::setupBackground()
{
    m_backgroundLabel = new QLabel(this);
    m_backgroundLabel->setScaledContents(true);

    m_bgMovie = new QMovie(Config::UI::PATH_MENU_BG);
    m_bgMovie->setSpeed(Config::UI::BG_ANIM_SPEED);

    m_backgroundLabel->setMovie(m_bgMovie);

    m_backgroundLabel->lower();
    m_backgroundLabel->setGeometry(0, 0, width(), height());
    m_bgMovie->start();
}

void MenuScreen::updatePanelContent(const QString &title, const QString &description)
{
    m_panelTitle->setText(title);
    m_panelText->setText(description);
    if (!m_sidePanel->isVisible())
    {
        m_sidePanel->show();
    }
}
