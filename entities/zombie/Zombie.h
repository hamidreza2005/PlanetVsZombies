#ifndef PLANETVSZOMBIES_ZOMBIE_H
#define PLANETVSZOMBIES_ZOMBIE_H

#include <QTimer>
#include <QPropertyAnimation>
#include "../GameEntity.h"

class Zombie : public GameEntity{
public:
    explicit Zombie(int health, float movementDelay, int attackPower, float firingRate,int brain);


    float getMovementDelay() const;
    void setMovementDelay(float newMovementDelay);
    int getAttackPower() const;
    void setAttackPower(int newAttackPower);
    float getFiringRate() const;
    void setFiringRate(float newFiringRate);

    void reduceHealth(int amount);

    virtual ~Zombie();
protected slots:
    void attack();
    virtual void move();
protected:
private:
    int health;
    float movementDelay;
    int attackPower;
    float firingRate;
    int brain;
    QTimer *attackTimer;
    QTimer *movementTimer;
    QPropertyAnimation *movementAnimation;

    void setUpTimers();
    void setUpAnimations();
};


#endif
