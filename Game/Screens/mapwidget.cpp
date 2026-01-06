#include "MapWidget.h"
#include "Map.h"
#include "Tile.h"
#include "InputManager.h"
#include "GameScreen.h"
MapWidget::MapWidget(QWidget* parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void MapWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    drawMap(painter);
}

void MapWidget::drawMap(QPainter& painter)
{
    Location* location = Map::getInstance().getCurrentLocation();
    if (!location)
    {
        painter.drawText(rect(), Qt::AlignCenter, "Map not loaded");
        return;
    }

    const auto& tiles = location->getTiles();
    int rows = tiles.size();
    int cols = tiles[0].size();

    int tileSize = qMin(width() / cols, height() / rows);

    for (int y = 0; y < rows; ++y)
    {
        for (int x = 0; x < cols; ++x)
        {
            QRect r(x * tileSize, y * tileSize, tileSize, tileSize);
            Tile* tile = tiles[y][x];

            QColor bg = tile->isTraversable() ? QColor("#004D40") : QColor("#3E2723");
            painter.fillRect(r, bg);
            painter.setPen(QColor("#1B5E20"));
            painter.drawRect(r);

            if (tile->getEntity())
            {
                painter.setPen(Qt::white);
                painter.drawText(r, Qt::AlignCenter,
                                 QString::fromStdString(tile->getEntity()->getSymbol()));
            }
        }
    }
}

void MapWidget::mousePressEvent(QMouseEvent* event)
{   if (event->button() == Qt::RightButton)
    {
        m_selectedUnit = nullptr;
        m_moveMode = false;
        update();
    }
    Location* location = Map::getInstance().getCurrentLocation();
    if (!location)
        return;

    const auto& tiles = location->getTiles();
    int rows = tiles.size();
    int cols = tiles[0].size();
    int tileSize = qMin(width() / cols, height() / rows);

    int x = event->pos().x() / tileSize;
    int y = event->pos().y() / tileSize;

    if (x < 0 || y < 0 || x >= cols || y >= rows)
        return;

    emit tileClicked(x, y);
}




