#include "LoadGameScreen.h"

LoadGameScreen::LoadGameScreen(QWidget *parent) : AbstractScreen(parent)
{
    setupUI();
    refreshSaveList();
}

void LoadGameScreen::onEnter() { this->show(); }

void LoadGameScreen::onExit() { this->hide(); }

void LoadGameScreen::setupUI()
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(100, 50, 100, 50);

    QLabel *header = new QLabel(tr("RE-ESTABLISH COMMAND UPLINK"), this);
    header->setObjectName("settingsTitle");
    layout->addWidget(header);

    m_saveList = new QListWidget(this);
    m_saveList->setObjectName("saveListWidget");
    layout->addWidget(m_saveList);

    m_detailsLabel = new QLabel(tr("Select a deployment zone..."), this);
    m_detailsLabel->setObjectName("statusLabel");
    layout->addWidget(m_detailsLabel);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    m_loadBtn = new QPushButton(tr("RESUME MISSION"));
    m_loadBtn->setEnabled(false);

    QPushButton *backBtn = new QPushButton(tr("BACK"));

    btnLayout->addWidget(backBtn);
    btnLayout->addWidget(m_loadBtn);
    layout->addLayout(btnLayout);

    connect(m_saveList, &QListWidget::itemClicked, this, &LoadGameScreen::onEntrySelected);
    connect(m_loadBtn, &QPushButton::clicked, this, &LoadGameScreen::onLoadClicked);
    connect(backBtn, &QPushButton::clicked, []()
            { MenuManager::getInstance().popScreen(); });
}

void LoadGameScreen::refreshSaveList()
{
    m_saveList->clear();
    QString saveRoot = QCoreApplication::applicationDirPath() + "/saves";
    QDir dir(saveRoot);

    QStringList folders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QString &folder : folders)
    {
        QListWidgetItem *item = new QListWidgetItem(folder);

        m_saveList->addItem(item);
    }
}

void LoadGameScreen::onEntrySelected(QListWidgetItem *item)
{
    m_loadBtn->setEnabled(true);
    m_detailsLabel->setText(tr("Target Sector: ") + item->text());
}

void LoadGameScreen::onLoadClicked()
{
    if (!m_saveList->currentItem())
        return;

    QString selectedMap = m_saveList->currentItem()->text();

    auto *loading = new LoadingScreen();
    MenuManager::getInstance().setScreen(loading);

    QTimer::singleShot(100, loading, [selectedMap, loading]()
                       {
        loading->setStatus(tr("RETRIEVING SECTOR DATA..."));
        loading->setProgress(25);

        QTimer::singleShot(200, loading, [selectedMap, loading]() {
            loading->setStatus(tr("RECONSTRUCTING TERRAIN..."));
            loading->setProgress(60);
            
            GameEngine::getInstance().loadMatch(selectedMap);

            QTimer::singleShot(200, loading, [loading]() {
                loading->setStatus(tr("SYNCHRONIZING UPLINK..."));
                loading->setProgress(100);
                
                QTimer::singleShot(500, loading, []() {
                    auto* game = new GameScreen();
                    MenuManager::getInstance().setScreen(game);
                    
                    
                    QTimer::singleShot(0, game, [game]() {
                        game->setFocus();
                        game->update();
                    });
                });
            });
        }); });
}
