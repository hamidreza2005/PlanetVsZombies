#ifndef PLANETVSZOMBIES_CARD_H
#define PLANETVSZOMBIES_CARD_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <functional>
#include "entities/GameEntity.h"
#include <QGraphicsTextItem>

class Card : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit Card(std::function<GameEntity*()> entityFactory, int width = 100, int height = 100, QGraphicsItem* parent = nullptr);
    std::function<GameEntity*()> getEntityFactory();
    void select();
    void unselect();
    int getCost() const;
    void disable();
    void enable();
signals:
    void selectEntity(Card* card);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void setImage();
    void setBorders();

private:
    std::function<GameEntity*()> entityFactory;
    GameEntity* entity;
    QGraphicsRectItem* border;
    QGraphicsTextItem* costText;
    int width;
    int height;

    void addCostLabel();
};

#endif // PLANETVSZOMBIES_CARD_H
