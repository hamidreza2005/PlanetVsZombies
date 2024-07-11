#include "TallZombie.h"
#include "../plant/Plant.h"
#include "../plant/Walnut.h"

TallZombie::TallZombie()
    : Zombie(
        INITIAL_HEALTH,
        INITIAL_MOVEMENT_DELAY,
        INITIAL_ATTACK_POWER,
        INITIAL_FIRING_RATE,
        BRAIN_NEEDED_TO_CREATE
        ) {
    this->setImage();
}

QString TallZombie::getPicturePath() const {
    return ":/resources/images/zombies/tall zombie.png";
}

QString TallZombie::getName() {
    return "Tall Zombie";
}

QString TallZombie::getMovementPicturePath() const {
    return ":/resources/images/move/TallZombieMove.png";
}

QString TallZombie::getStayPicturePath() const {
    return ":/resources/images/move/TallZombieStay.png";
}

QString TallZombie::getCardPicturePath() const {
    return ":/resources/images/cards/tallcard.png";
}

bool TallZombie::isThereAPlantInWay() {
    QList<QGraphicsItem*> collidingItems = this->collidingItems();
    for (QGraphicsItem* item : collidingItems) {
        auto plant = dynamic_cast<Plant*>(item);
        auto isWalnut = (bool) dynamic_cast<Walnut*>(item);
        if (isWalnut){
            if (!this->hasRisen) {
                this->setY(y() - 78);
                this->hasRisen = true;
            }
            return false;
        }
        if (this->hasRisen){
            this->setY(y() + 78);
            this->hasRisen = false;
        }
        if (plant) {
            return true;
        }
    }
    return false;
}