#include "Bullet.h"
#include "../zombie/Zombie.h"
#include <QGraphicsScene>
#include <QList>
#include <QDebug>

Bullet::Bullet(int attackPower, int speed, QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(parent), attackPower(attackPower), speed(speed) {
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Bullet::move);
    timer->start(50);
}

void Bullet::setImage(const QString& imagePath) {
    QPixmap image(imagePath);
    QPixmap scaledImage = image.scaled(30, 30, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    setPixmap(scaledImage);
}

void Bullet::move() {
    if (!scene()) {
        return;
    }

    QList<QGraphicsItem*> collidingItems = this->collidingItems();
    for (QGraphicsItem* item : collidingItems) {
        Zombie* zombie = dynamic_cast<Zombie*>(item);
        if (zombie) {
            zombie->reduceHealth(attackPower);
            scene()->removeItem(this);
            delete this;
            return;
        }
    }

    setPos(x() + speed, y());

    if (x() > 1000) {
        if (scene()) {
            scene()->removeItem(this);
        }
        delete this;
    }
}
