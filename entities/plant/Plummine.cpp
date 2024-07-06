#include "PlumMine.h"
#include <QGraphicsScene>
#include "../zombie/Zombie.h"

PlumMine::PlumMine() :
    Plant(INITIAL_HEALTH,SUN_NEEDED_TO_CREATE
, INITIAL_FIRING_RATE, INITIAL_ATTACK_POWER)
{
    this->setImage();
}

QString PlumMine::getPicturePath() const {
    return ":/resources/images/plants/plum mine.png";
}

QString PlumMine::getName() {
    return "Plum Mine Plant";
}

void PlumMine::fire() {
    QList<QGraphicsItem*> collidingItems = this->collidingItems();
    for (QGraphicsItem* item : collidingItems) {
        Zombie* zombie = dynamic_cast<Zombie*>(item);
        if (zombie) {
            zombie->reduceHealth(this->attackPower);
        }
    }
    scene()->removeItem(this);
    delete this;
}
