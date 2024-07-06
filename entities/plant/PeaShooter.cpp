#include "PeaShooter.h"
#include "Bullet.h"
#include "../zombie/Zombie.h"
#include <QGraphicsScene>
#include <QDebug>

PeaShooter::PeaShooter() :
    Plant(INITIAL_HEALTH, 100, INITIAL_FIRING_RATE, INITIAL_ATTACK_POWER)
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
       // qDebug() << "PeaShooter is not in a scene";
        return;
    }

    Bullet* bullet = new Bullet(attackPower, 10);
    bullet->setImage(":/resources/images/sun.png");
    bullet->setPos(x() + 30, y() + 15);
    scene()->addItem(bullet);
}
