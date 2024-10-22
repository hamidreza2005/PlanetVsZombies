#include "Bullet.h"
#include "../zombie/Zombie.h"
#include <QGraphicsScene>
#include <QList>

Bullet::Bullet(int attackPower, int speed,bool canHitMultipleZombies,int size, QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(parent), attackPower(attackPower), speed(speed), size(size), canHitMultipleZombies(canHitMultipleZombies) {
    this->setImage(":/resources/images/bullet1.png");
    this->hitZombies = new QList<Zombie*>();
    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &Bullet::move);
    movementTimer->start(50);
}

void Bullet::setImage(const QString& imagePath) {
    QPixmap image(imagePath);
    QPixmap scaledImage = image.scaled(this->size, this->size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
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
        if (hitZombies->contains(zombie)){
            continue;
        }

        zombie->setHealth(zombie->getHealth() - attackPower);

        if (this->canHitMultipleZombies){
            hitZombies->append(zombie);
        }

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
    delete hitZombies;
    movementTimer->stop();
    delete movementTimer;
}
