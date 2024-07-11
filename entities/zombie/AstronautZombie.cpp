#include "AstronautZombie.h"

AstronautZombie::AstronautZombie()
    : Zombie(
        INITIAL_HEALTH,
        INITIAL_MOVEMENT_DELAY,
        INITIAL_ATTACK_POWER,
        INITIAL_FIRING_RATE,
        BRAIN_NEEDED_TO_CREATE
        ) {
    this->setImage();
}

QString AstronautZombie::getPicturePath() const {
    return ":/resources/images/zombies/astronaut zombie.png";
}

QString AstronautZombie::getName() {
    return "Astronaut Zombie";
}

QString AstronautZombie::getMovementPicturePath() const {
    return ":/resources/images/move/AstronautZombieMove.png";
}

QString AstronautZombie::getStayPicturePath() const {
    return ":/resources/images/move/AstronautZombieStay.png";
}

void AstronautZombie::move() {
    if (this->getHealth() <= 100) {
        this->setMovementDelay(0.5);
        this->setFiringRate(0.5);
    }
    Zombie::move();
}

QString AstronautZombie::getCardPicturePath() const {
    return ":/resources/images/cards/astronautcard.png";
}
