#include "PlumMine.h"
#include <QGraphicsScene>
#include "../zombie/Zombie.h"
#include "explosionutils.h"


PlumMine::PlumMine() :
    Plant(INITIAL_HEALTH, SUN_NEEDED_TO_CREATE, INITIAL_FIRING_RATE, INITIAL_ATTACK_POWER)
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
    if (!scene()) {
        return;
    }
    qreal rowY = this->y();
    qreal colX = this->x();

    QList<QGraphicsItem*> allItems = scene()->items();
    for (QGraphicsItem* item : allItems) {
        Zombie* zombie = dynamic_cast<Zombie*>(item);
        if (zombie) {
            if (qAbs(zombie->y() - rowY) <= 100.0 && qAbs(zombie->x() - colX) <= 100.0) {
                zombie->setHealth(zombie->getHealth() - this->attackPower);
                if(zombie->getHealth() < 0){
                    scene()->removeItem(zombie);
                    delete zombie;
                }
            }
        }
    }

    showExplosionImage(scene(), this->pos());

    scene()->removeItem(this);
    delete this;
}

bool PlumMine::isThereAZombieInTheRow() {
    if (!scene()) {
        return false;
    }
    return true;
}

QString PlumMine::getCardPicturePath() const {
    return ":/resources/images/cards/plumminecard.png";
}

