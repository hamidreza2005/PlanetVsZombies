#include "Zombie.h"

Zombie::Zombie(int health, float movementDelay, int attackPower,float firingRate):
GameEntity(health),
movementDelay(movementDelay),
attackPower(attackPower),
firingRate(firingRate)
{
    this->setZValue(100);
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
//    qDebug() << "zombie attacked";
}

Zombie::~Zombie() {
    delete this->movementTimer;
    delete this->attackTimer;
    delete this->movementAnimation;
}

void Zombie::move() {
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
    this->attackTimer->start(this->firingRate * 1000);
    this->movementTimer->start(this->movementDelay * 1000);
}

void Zombie::setUpAnimations() {
    this->movementAnimation = new QPropertyAnimation(this, "x");
    this->movementAnimation->setDuration(1000);
}

void Zombie::setImage() {
    QPixmap image_ground(getPicturePath());
    QPixmap Scaled_image_ground = image_ground.scaled(75,75,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    setPixmap(Scaled_image_ground);
}
