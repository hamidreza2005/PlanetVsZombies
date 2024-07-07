#include "Zombie.h"
#include "../plant/Plant.h"

Zombie::Zombie(int health, float movementDelay, int attackPower,float firingRate,int brain):
GameEntity(health),
movementDelay(movementDelay),
attackPower(attackPower),
firingRate(firingRate),
brain(brain)
{
    this->setZValue(10);
    this->setUpTimers();
    this->setUpAnimations();
}

float Zombie::getMovementDelay() const {
    return movementDelay;
}
void Zombie::setMovementDelay(float newMovementDelay) {
    movementDelay = newMovementDelay;
}

int Zombie::getAttackPower() const {
    return attackPower;
}

void Zombie::setAttackPower(int newAttackPower) {
    attackPower = newAttackPower;
}

float Zombie::getFiringRate() const {
    return firingRate;
}

void Zombie::setFiringRate(float newFiringRate) {
    this->firingRate = newFiringRate;
}

void Zombie::reduceHealth(int amount) {
    this->health -= amount;
}

void Zombie::attack(){
    if (!this->isThereAPlantInWay()){
        this->activateMovementMove();
        return;
    }

    QList<QGraphicsItem*> collidingItems = this->collidingItems();
    for (QGraphicsItem* item : collidingItems) {
        auto plant = dynamic_cast<Plant*>(item);
        if (!plant) {
            continue;
        }
        plant->setHealth(plant->getHealth() - this->attackPower);
        if(plant->getHealth() < 0){
            delete plant;
        }
    }
}

Zombie::~Zombie() {
    this->movementTimer->stop();
    this->attackTimer->stop();
    this->movementAnimation->stop();
    delete this->movementTimer;
    delete this->attackTimer;
    delete this->movementAnimation;
}

void Zombie::move() {
    if (this->isThereAPlantInWay()){
        this->activateAttackMode();
        return;
    }
    if(this->x() < -150){
        delete this;
        return;
    }
    this->movementAnimation->stop();
    this->movementAnimation->setStartValue(this->x());
    this->movementAnimation->setEndValue(this->x() - 20);
    this->movementAnimation->start();
}

void Zombie::setUpTimers() {
    this->attackTimer = new QTimer();
    this->movementTimer = new QTimer();
    connect(attackTimer,&QTimer::timeout,this,&Zombie::attack);
    connect(movementTimer,&QTimer::timeout,this,&Zombie::move);
    this->movementTimer->start(this->movementDelay * 1000);
}

void Zombie::setUpAnimations() {
    this->movementAnimation = new QPropertyAnimation(this, "x");
    this->movementAnimation->setDuration(this->movementDelay * 1000);
}

bool Zombie::isThereAPlantInWay() {
    QList<QGraphicsItem*> collidingItems = this->collidingItems();
    for (QGraphicsItem* item : collidingItems) {
        auto plant = dynamic_cast<Plant*>(item);
        if (plant) {
            return true;
        }
    }
    return false;
}

void Zombie::activateAttackMode() {
    this->attackTimer->start(this->firingRate * 1000);
    this->movementAnimation->stop();
    this->movementTimer->stop();
}

void Zombie::activateMovementMove() {
    this->movementTimer->start(this->movementDelay * 1000);
    this->attackTimer->stop();
}
