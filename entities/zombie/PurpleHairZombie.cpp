#include "PurpleHairZombie.h"

PurpleHairZombie::PurpleHairZombie():
Zombie(
        INITIAL_HEALTH,
        INITIAL_MOVEMENT_DELAY,
        INITIAL_ATTACK_POWER,
        INITIAL_FIRING_RATE,
        BRAIN_NEEDED_TO_CREATE
    )
{
    this->setImage();
}

QString PurpleHairZombie::getPicturePath() const {
    return ":/resources/images/zombies/purple hair zombie.png";
}

QString PurpleHairZombie::getName() {
    return "Purple Head Zombie";
}