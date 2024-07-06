#include "Walnut.h"
#include <QGraphicsScene>

Walnut::Walnut() :
    Plant(INITIAL_HEALTH, 100, INITIAL_FIRING_RATE, INITIAL_ATTACK_POWER)
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
