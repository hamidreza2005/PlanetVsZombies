#include "RegularZombie.h"

RegularZombie::RegularZombie():
Zombie(INITIAL_HEALTH,INITIAL_MOVEMENT_DELAY,INITIAL_ATTACK_POWER,INITIAL_FIRING_RATE,BRAIN_NEEDED_TO_CREATE)
{
    this->setImage();
}

QString RegularZombie::getPicturePath() const {
    return ":/resources/images/zombies/regular zombie.png";
}

QString RegularZombie::getName() {
    return "Regular Zombie";
}
