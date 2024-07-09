#include "TallZombie.h"

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
