#include "Boomrang.h"
#include "Bullet.h"
#include "../zombie/Zombie.h"
#include <QGraphicsScene>
#include <QDebug>

Boomerang::Boomerang() :
    Plant(INITIAL_HEALTH,                 SUN_NEEDED_TO_CREATE
            , INITIAL_FIRING_RATE, INITIAL_ATTACK_POWER)
{
    this->setImage();
}

QString Boomerang::getPicturePath() const {
    return ":/resources/images/plants/boomrang.png";
}

QString Boomerang::getName() {
    return "Boomerang Plant";
}

void Boomerang::fire() {
    if (!scene()) {
       // qDebug() << "Boomerang is not in a scene";
        return;
    }

    Bullet* bullet = new Bullet(attackPower, 10);
    bullet->setImage(":/resources/images/sun.png");
    bullet->setPos(x() + 30, y() + 15);
    scene()->addItem(bullet);
}
