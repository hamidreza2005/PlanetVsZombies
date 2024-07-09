#include "Boomrang.h"
#include "Bullet.h"
#include "../zombie/Zombie.h"
#include <QGraphicsScene>
#include <QDebug>

Boomerang::Boomerang() :
    Plant(INITIAL_HEALTH, SUN_NEEDED_TO_CREATE, INITIAL_FIRING_RATE, INITIAL_ATTACK_POWER),
    currentImageIndex(0) {
    this->setImage();
    bulletImages << ":/resources/images/boomrang1.png"
                 << ":/resources/images/boomrang2.png"
                 << ":/resources/images/boomrang3.png"
                 << ":/resources/images/boomrang4.png";
    imageChangeTimer = new QTimer(this);
    connect(imageChangeTimer, &QTimer::timeout, this, &Boomerang::changeBulletImage);
    imageChangeTimer->start(1000);
}

QString Boomerang::getPicturePath() const {
    return ":/resources/images/plants/boomrang.png";
}

QString Boomerang::getName() {
    return "Boomerang Plant";
}

void Boomerang::fire() {
    if (!scene()) {
        return;
    }

    auto bullet = new Bullet(attackPower, 10, true);
    bullet->setImage(bulletImages[currentImageIndex]);
    bullet->setPos(x() + 30, y() + 15);
    scene()->addItem(bullet);

    connect(this, &Boomerang::imageChanged, bullet, &Bullet::setImage);
}

void Boomerang::changeBulletImage() {
    currentImageIndex = (currentImageIndex + 1) % bulletImages.size();
    emit imageChanged(bulletImages[currentImageIndex]);
}
