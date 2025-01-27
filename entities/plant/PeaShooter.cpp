#include "PeaShooter.h"
#include "Bullet.h"
#include "../zombie/Zombie.h"
#include <QGraphicsScene>
#include <QDebug>

PeaShooter::PeaShooter() :
    Plant(INITIAL_HEALTH,SUN_NEEDED_TO_CREATE
, INITIAL_FIRING_RATE, INITIAL_ATTACK_POWER)
{
    this->setImage();
}

QString PeaShooter::getPicturePath() const {
    return ":/resources/images/plants/peashooter.png";
}

QString PeaShooter::getName() {
    return "PeaShooter Plant";
}

void PeaShooter::fire() {
    if (!scene()) {
        return;
    }

    Bullet* bullet = new Bullet(attackPower);
    bullet->setPos(x() + 30, y() + 15);
    scene()->addItem(bullet);
}

QString PeaShooter::getCardPicturePath() const {
    return ":/resources/images/cards/peashootercard.png";
}
