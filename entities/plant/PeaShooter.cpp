#include "PeaShooter.h"

PeaShooter::PeaShooter():
Plant(INITIAL_HEALTH,50,INITIAL_FIRING_RATE,INITIAL_ATTACK_POWER)
{
    this->setImage();
}

QString PeaShooter::getPicturePath() const {
    return ":/resources/images/plants/peashooter.png";
}

QString PeaShooter::getName() {
    return "PeaShooter Plant";
}
