#include "Jalapeno.h"
#include <QGraphicsScene>
#include "../zombie/Zombie.h"
#include <QGraphicsScene>

Jalapeno::Jalapeno() :
    Plant(INITIAL_HEALTH, 150, INITIAL_FIRING_RATE, INITIAL_ATTACK_POWER)
{
    this->setImage();
}

QString Jalapeno::getPicturePath() const {
    return ":/resources/images/plants/jalapino.png";
}

QString Jalapeno::getName() {
    return "Jalapeno Plant";
}

void Jalapeno::fire() {
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
