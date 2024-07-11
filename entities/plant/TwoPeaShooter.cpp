#include "TwoPeaShooter.h"
#include "Bullet.h"
#include "../zombie/Zombie.h"
#include <QGraphicsScene>
#include <QDebug>

TwoPeaShooter::TwoPeaShooter() :
    Plant(INITIAL_HEALTH,SUN_NEEDED_TO_CREATE
, INITIAL_FIRING_RATE, INITIAL_ATTACK_POWER)
{
    this->setImage();
}

QString TwoPeaShooter::getPicturePath() const {
    return ":/resources/images/plants/two_peashooter.png";
}

QString TwoPeaShooter::getName() {
    return "TwoPeaShooter Plant";
}

void TwoPeaShooter::fire() {
    if (!scene()) {
        return;
    }

    Bullet* bullet = new Bullet(attackPower, 10);
    bullet->setImage(":/resources/images/bullet2.png");
    bullet->setPos(x() + 30, y() + 15);
    scene()->addItem(bullet);
}

QString TwoPeaShooter::getCardPicturePath() const {
    return ":/resources/images/cards/twopeashootercard.png";
}
