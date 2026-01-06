#ifndef MAPWIDGET_H
#define MAPWIDGET_H
#pragma once
#include <QWidget>
#include <QPainter>
#include "unit_builder/Unit.h"
#include "../Map/Location.h"
#include "MainMenuScreen.h"
#include "../Map/Tile.h"
#include "../Map/Map.h"

class MapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MapWidget(QWidget* parent = nullptr);
    Location *location = Map::getInstance().getCurrentLocation();
protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent* event) override;
signals:
    void tileClicked(int x, int y);
private:
    void drawMap(QPainter& painter);
    UnitType m_selectedUnitType;

    PlacementState m_placementState = PlacementState::NONE;
    UnitType m_pendingUnit = UnitType::NONE;

    bool m_moveMode = false;
    Unit* m_selectedUnit = nullptr;
};


#endif // MAPWIDGET_H
