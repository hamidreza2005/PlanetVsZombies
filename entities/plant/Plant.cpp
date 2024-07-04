#include "Plant.h"

Plant::Plant(int health,int sun,float firingRate,int attackPower):
GameEntity(health),
sun(sun),
firingRate(firingRate),
attackPower(attackPower)
{
    this->setUpTimers();
}

void Plant::fire() {

}

void Plant::setUpTimers() {
    if (this->firingRate == 0){
        return;
    }
    this->fireTimer = new QTimer();
    connect(fireTimer,&QTimer::timeout,this,&Plant::fire);
    this->fireTimer->start(firingRate * 1000);
}