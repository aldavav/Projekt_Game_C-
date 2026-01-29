#include "LoadGameScreen.h"

LoadGameScreen::LoadGameScreen(QWidget *parent) : AbstractScreen(parent)
{
    setupUI();
    refreshSaveList();
}

void LoadGameScreen::onEnter()
{
    this->show();
    m_saveList->setFocus();

    if (m_saveList->count() > 0 && !m_saveList->currentItem())
    {
        m_saveList->setCurrentRow(0);
    }
}

void LoadGameScreen::onLoadClicked()
{
    if (!m_saveList->currentItem())
        return;

    m_loadBtn->setEnabled(false);

    QString selectedMap = m_saveList->currentItem()->text();
    auto *loading = new LoadingScreen();
    MenuManager::getInstance().setScreen(loading);

    QTimer::singleShot(Config::UI::LOAD_STEP_RETRIEVE, loading, [selectedMap, loading]()
                       {
        loading->setStatus(tr("RETRIEVING SECTOR DATA..."));
        loading->setProgress(25);

        QTimer::singleShot(Config::UI::LOAD_STEP_RECONSTRUCT, loading, [selectedMap, loading]() {
            loading->setStatus(tr("RECONSTRUCTING TERRAIN..."));
            loading->setProgress(60);
            
            GameEngine::getInstance().loadMatch(selectedMap);

            QTimer::singleShot(Config::UI::LOADING_STEP_DELAY, loading, [loading]() {
                loading->setStatus(tr("SYNCHRONIZING UPLINK..."));
                loading->setProgress(100);
                
                QTimer::singleShot(Config::UI::LOADING_FINAL_PAUSE, loading, []() {
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

void LoadGameScreen::onBackClicked()
{
    MenuManager::getInstance().popScreen();
}

void LoadGameScreen::onEntrySelected(QListWidgetItem *item)
{
    if (!item)
        return;

    m_loadBtn->setEnabled(true);
    m_deleteBtn->setEnabled(true);

    m_detailsLabel->setText(tr("> SECTOR IDENTIFIED: ") + item->text().toUpper());
}

void LoadGameScreen::onDeleteClicked()
{
    if (!m_saveList->currentItem())
        return;

    QString selectedMap = m_saveList->currentItem()->text();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("ERASE DATA"),
                                  tr("Confirm deletion of deployment data for: ") + selectedMap,
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        QString savePath = QCoreApplication::applicationDirPath() +
                           Config::Paths::SAVE_DIR_NAME + "/" + selectedMap;

        QDir dir(savePath);
        if (dir.exists())
        {
            dir.removeRecursively();
            refreshSaveList();
            m_detailsLabel->setText(tr("> DATA PURGED SUCCESSFULLY."));
            m_loadBtn->setEnabled(false);
            m_deleteBtn->setEnabled(false);
        }
    }
}

void LoadGameScreen::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        MenuManager::getInstance().popScreen();
    }
    else if (event->key() == Qt::Key_Delete)
    {
        if (m_deleteBtn->isEnabled())
            onDeleteClicked();
    }
    else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        if (m_loadBtn->isEnabled())
            onLoadClicked();
    }
    else
    {
        AbstractScreen::keyPressEvent(event);
    }
}

void LoadGameScreen::setupUI()
{
    setObjectName("loadGameScreen");
    auto *layout = new QVBoxLayout(this);

    layout->setContentsMargins(Config::UI::LIST_SCREEN_MARGIN_H, Config::UI::LIST_SCREEN_MARGIN_V,
                               Config::UI::LIST_SCREEN_MARGIN_H, Config::UI::LIST_SCREEN_MARGIN_V);

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
    m_loadBtn->setObjectName("applyButton");
    m_loadBtn->setEnabled(false);

    QPushButton *backBtn = new QPushButton(tr("BACK"));
    backBtn->setObjectName("cancelButton");

    m_deleteBtn = new QPushButton(tr("ABORT SAVED DATA"));
    m_deleteBtn->setObjectName("cancelButton");
    m_deleteBtn->setEnabled(false);

    btnLayout->addWidget(backBtn);
    btnLayout->addWidget(m_deleteBtn);
    btnLayout->addWidget(m_loadBtn);
    layout->addLayout(btnLayout);

    connect(m_saveList, &QListWidget::currentItemChanged, this, [this](QListWidgetItem *current, QListWidgetItem *previous)
            {
        if (current) onEntrySelected(current); });
    connect(m_loadBtn, &QPushButton::clicked, this, &LoadGameScreen::onLoadClicked);
    connect(backBtn, &QPushButton::clicked, []()
            { MenuManager::getInstance().popScreen(); });
    connect(m_deleteBtn, &QPushButton::clicked, this, &LoadGameScreen::onDeleteClicked);
}

void LoadGameScreen::refreshSaveList()
{
    m_saveList->clear();

    QString saveRoot = QCoreApplication::applicationDirPath() + Config::Paths::SAVE_DIR_NAME;
    QDir dir(saveRoot);

    if (!dir.exists())
        dir.mkpath(".");

    QStringList folders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    QList<QPair<QDateTime, QString>> saveEntries;

    for (const QString &folder : folders)
    {
        QString filePath = dir.absoluteFilePath(folder + Config::Paths::INITIAL_SAVE_FILENAME);
        QFileInfo fileInfo(filePath);

        if (fileInfo.exists())
        {
            saveEntries.append({fileInfo.lastModified(), folder});
        }
        else
        {
            saveEntries.append({QFileInfo(dir.absoluteFilePath(folder)).lastModified(), folder});
        }
    }

    std::sort(saveEntries.begin(), saveEntries.end(),
              [](const QPair<QDateTime, QString> &a, const QPair<QDateTime, QString> &b)
              {
                  return a.first > b.first;
              });

    for (const auto &entry : saveEntries)
    {
        QListWidgetItem *item = new QListWidgetItem(entry.second);

        item->setToolTip(tr("Last Played: ") + entry.first.toString("yyyy-MM-dd HH:mm"));
        m_saveList->addItem(item);
    }
}
