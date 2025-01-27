#include "LeafHeadZombie.h"

LeafHeadZombie::LeafHeadZombie()
    : Zombie(
        INITIAL_HEALTH,
        INITIAL_MOVEMENT_DELAY,
        INITIAL_ATTACK_POWER,
        INITIAL_FIRING_RATE,
        BRAIN_NEEDED_TO_CREATE
        ) {
    this->setImage();
}

QString LeafHeadZombie::getPicturePath() const {
    return ":/resources/images/zombies/leaf hair zombie.png";
}

QString LeafHeadZombie::getName() {
    return "Leaf Head Zombie";
}

QString LeafHeadZombie::getMovementPicturePath() const {
    return ":/resources/images/move/LeafHeadZombieMove.png";
}

QString LeafHeadZombie::getStayPicturePath() const {
    return ":/resources/images/move/LeafHeadZombieStay.png";
}

QString LeafHeadZombie::getCardPicturePath() const {
    return ":/resources/images/cards/leafheadcard.png";
}
