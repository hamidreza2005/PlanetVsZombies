#include "Plant.h"
#include "../zombie/Zombie.h"
#include <QGraphicsScene>

Plant::Plant(int health, int sun, float firingRate, int attackPower) :
    GameEntity(health,sun),
    sun(sun),
    firingRate(firingRate),
    attackPower(attackPower)
{
    this->setUpTimers();
}

void Plant::fireImpl() {
    if(isThereAZombieInTheRow()){
        fire();
    }
}

void Plant::setUpTimers() {
    if (this->firingRate == 0) {
        return;
    }
    this->fireTimer = new QTimer();
    connect(fireTimer, &QTimer::timeout, this, &Plant::fireImpl);
    this->fireTimer->start(firingRate * 1000);
}

Plant::~Plant() {
    this->fireTimer->stop();
    delete this->fireTimer;
}

bool Plant::isThereAZombieInTheRow() {
    if (!scene()){
        return false;
    }

    QList<QGraphicsItem*> allItems = scene()->items();
    for (auto item : allItems) {
        auto zombie = dynamic_cast<Zombie*>(item);
        if (zombie && qAbs(zombie->y() - this->y()) <= 10.0) {
            return true;
        }
    }
    return false;
}
