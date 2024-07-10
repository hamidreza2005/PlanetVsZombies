#ifndef PLANETVSZOMBIES_ZOMBIE_H
#define PLANETVSZOMBIES_ZOMBIE_H

#include <QTimer>
#include <QPropertyAnimation>
#include "../GameEntity.h"

class Zombie : public GameEntity {
    Q_OBJECT
public:
    explicit Zombie(int health, float movementDelay, int attackPower, float firingRate, int brain);

    float getMovementDelay() const;
    void setMovementDelay(float newMovementDelay);
    int getAttackPower() const;
    void setAttackPower(int newAttackPower);
    float getFiringRate() const;
    void setFiringRate(float newFiringRate);
    void reduceHealth(int amount);

    virtual QString getMovementPicturePath() const = 0;
    virtual QString getStayPicturePath() const = 0;

    virtual ~Zombie();
protected slots:
    void attack();
    virtual void move();
    void switchImage();
protected:
private:
    int health;
    float movementDelay;
    int attackPower;
    float firingRate;
    int brain;
    QTimer* attackTimer;
    QTimer* movementTimer;
    QTimer* imageSwitchTimer;
    QPropertyAnimation* movementAnimation;
    bool isMovingImage;

    void setUpTimers();
    void setUpAnimations();
    bool isThereAPlantInWay();
    void activateAttackMode();
    void activateMovementMode();
    bool hasReachedTheEndOfTheGround();
signals:
    void zombieReachedToTheEnd();
};

#endif
