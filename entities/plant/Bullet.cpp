#include "Bullet.h"
#include "../zombie/Zombie.h"
#include <QGraphicsScene>
#include <QList>

Bullet::Bullet(int attackPower, int speed,bool canHitMultipleZombies, QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(parent), attackPower(attackPower), speed(speed), canHitMultipleZombies(canHitMultipleZombies) {
    this->setImage(":/resources/images/bullet1.png");
    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &Bullet::move);
    movementTimer->start(50);
}

void Bullet::setImage(const QString& imagePath) {
    QPixmap image(imagePath);
    QPixmap scaledImage = image.scaled(20, 20, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    setPixmap(scaledImage);
}

void Bullet::move() {
    if (!scene()) {
        return;
    }

    QList<QGraphicsItem*> collidingItems = this->collidingItems();
    for (QGraphicsItem* item : collidingItems) {
        auto zombie = dynamic_cast<Zombie*>(item);
        if (!zombie) {
            continue;
        }

        zombie->setHealth(zombie->getHealth() - attackPower);
        if(zombie->getHealth() < 0){
            scene()->removeItem(zombie);
            delete zombie;
        }

        if (!this->canHitMultipleZombies){
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

Bullet::~Bullet() {
    delete movementTimer;
}
