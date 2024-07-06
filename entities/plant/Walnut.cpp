#include "Walnut.h"
#include <QGraphicsScene>

Walnut::Walnut() :
    Plant(INITIAL_HEALTH,  SUN_NEEDED_TO_CREATE
, INITIAL_FIRING_RATE, INITIAL_ATTACK_POWER)
{
    this->setImage();
}

QString Walnut::getPicturePath() const {
    return ":/resources/images/plants/walnut.png";
}

QString Walnut::getName() {
    return "Walnut Plant";
}

void Walnut::fire() {
}
