#ifndef PLANETVSZOMBIES_ZOMBIE_H
#define PLANETVSZOMBIES_ZOMBIE_H

#include <QTimer>
#include <QPropertyAnimation>
#include "../GameEntity.h"

class Zombie : public GameEntity{
    Q_PROPERTY(qreal x READ x WRITE setX)
public:
    explicit Zombie(int health, float movementDelay, int attackPower, float firingRate);

    // Getters and Setters

    float getMovementDelay() const;
    void setMovementDelay(float newMovementDelay);
    int getAttackPower() const;
    void setAttackPower(int newAttackPower);
    float getFiringRate() const;
    void setFiringRate(float newFiringRate);

    void reduceHealth(int amount);

    void setImage();
    virtual ~Zombie();
protected slots:
    void attack();
    void move();
protected:
private:
    int health;
    float movementDelay;
    int attackPower;
    float firingRate;
    QTimer *attackTimer;
    QTimer *movementTimer;
    QPropertyAnimation *movementAnimation;

    void setUpTimers();
    void setUpAnimations();
};


#endif
