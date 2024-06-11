#ifndef PLANETVSZOMBIES_ZOMBIE_H
#define PLANETVSZOMBIES_ZOMBIE_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QPropertyAnimation>

class Zombie : public QObject,public QGraphicsPixmapItem{
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX)
public:
    explicit Zombie(int health, float movementDelay, int attackPower, float firingRate);

    // Getters and Setters
    int getHealth() const;
    void setHealth(int newHealth);
    float getMovementDelay() const;
    void setMovementDelay(float newMovementDelay);
    int getAttackPower() const;
    void setAttackPower(int newAttackPower);
    float getFiringRate() const;
    void setFiringRate(float newFiringRate);

    void reduceHealth(int amount);

    // Abstract Methods
    virtual QString getName() = 0;
    virtual QString getPicturePath() const = 0;
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
