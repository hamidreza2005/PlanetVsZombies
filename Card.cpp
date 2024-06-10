#include "Card.h"
#include "QPixmap"
#include <QCursor>
#include <QPen>

void Card::setImage(const QString& src) {
    QPixmap image_ground(src);
    QPixmap Scaled_image_ground = image_ground.scaled(this->width,this->height,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    setPixmap(Scaled_image_ground);
    this->setCursor(Qt::PointingHandCursor);
}

void Card::setBorders(){
    auto* border = new QGraphicsRectItem(this);
    border->setRect(0, 0, this->width,this->height);
    border->setPen(QPen(Qt::green, 4));
    border->setZValue(-1);
}

Card::Card(int width,int height):width(width), height(height) {
    this->setBorders();
}
