#include "Jalapeno.h"
#include <QGraphicsScene>
#include "../zombie/Zombie.h"
#include <QGraphicsScene>

Jalapeno::Jalapeno() :
    Plant(INITIAL_HEALTH, SUN_NEEDED_TO_CREATE
, INITIAL_FIRING_RATE, INITIAL_ATTACK_POWER)
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
    if (!scene()) {
        return;
    }

    qreal rowY = this->y();
    QList<QGraphicsItem*> allItems = scene()->items();
    for (QGraphicsItem* item : allItems) {
        Zombie* zombie = dynamic_cast<Zombie*>(item);
        if (zombie) {
            if (qAbs(zombie->y() - rowY) <= 10.0) {
                zombie->setHealth(zombie->getHealth() - this->attackPower);
                if(zombie->getHealth() < 0){
                    scene()->removeItem(zombie);
                    delete zombie;
                }
            }
        }
    }

    scene()->removeItem(this);
    delete this;
}

bool Jalapeno::isThereAZombieInTheRow() {
    if (!scene()) {
        return false;
    }
    return true;
}

QString Jalapeno::getCardPicturePath() const {
    return ":/resources/images/cards/jalapenocard.png";
}
