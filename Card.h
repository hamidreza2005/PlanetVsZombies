#ifndef PLANETVSZOMBIES_CARD_H
#define PLANETVSZOMBIES_CARD_H
#include "QGraphicsPixmapItem"
#include "QObject"
#include "entities/GameEntity.h"
#include "QGraphicsSceneMouseEvent"

class Card : public QObject,public QGraphicsPixmapItem{
    Q_OBJECT
public:
    explicit Card(std::function<GameEntity*()> entityFactory,int width = 100,int height = 100);
    std::function<GameEntity*()> getEntityFactory();
    void select();
    void unselect();
protected:
    void setImage();
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
private:
    int width = 100;
    int height = 100;
    GameEntity* entity;
    std::function<GameEntity*()> entityFactory;
    QGraphicsRectItem* border;
    void setBorders();
signals:
    void selectEntity(Card* card);
};


#endif //PLANETVSZOMBIES_CARD_H
