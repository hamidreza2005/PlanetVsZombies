#include "BucketHeadZombie.h"

BucketHeadZombie::BucketHeadZombie()
    : Zombie(
        INITIAL_HEALTH,
        INITIAL_MOVEMENT_DELAY,
        INITIAL_ATTACK_POWER,
        INITIAL_FIRING_RATE,
        BRAIN_NEEDED_TO_CREATE
        ) {
    this->setImage();
}

QString BucketHeadZombie::getPicturePath() const {
    return ":/resources/images/zombies/Bucket head zombie.png";
}

QString BucketHeadZombie::getName() {
    return "Bucket Head Zombie";
}

QString BucketHeadZombie::getMovementPicturePath() const {
    return ":/resources/images/move/BucketHeadZombieMove.png";
}

QString BucketHeadZombie::getStayPicturePath() const {
    return ":/resources/images/move/BucketHeadZombieStay.png";
}

QString BucketHeadZombie::getCardPicturePath() const {
    return ":/resources/images/cards/bucketheadcard.png";
}
