#include "Card.h"
#include "QPixmap"
#include <QCursor>
#include <QPen>
#include "QGraphicsScene"

void Card::setImage() {
    QPixmap image_ground(this->entity->getPicturePath());
    QPixmap Scaled_image_ground = image_ground.scaled(this->width,this->height,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    setPixmap(Scaled_image_ground);
    this->setCursor(Qt::PointingHandCursor);
}

void Card::setBorders(){
    border = new QGraphicsRectItem(this);
    border->setRect(0, 0, this->width,this->height);
    border->setPen(QPen(Qt::green, 4));
    border->setZValue(-1);
}

Card::Card(std::function<GameEntity*()> entityFactory,int width,int height):entityFactory(entityFactory),width(width), height(height) {
    this->entity = entityFactory();
    this->setBorders();
    this->setImage();
}

void Card::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit selectEntity(this);
    }
    QGraphicsPixmapItem::mousePressEvent(event);
}

std::function<GameEntity *()> Card::getEntityFactory() {
    return this->entityFactory;
}

void Card::unselect() {
    this->border->setPen(QPen(Qt::green, 4));
}

void Card::select() {
    this->border->setPen(QPen(Qt::red, 4));
}
