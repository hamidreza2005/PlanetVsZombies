#ifndef PLANETVSZOMBIES_ASTRONAUTZOMBIE_H
#define PLANETVSZOMBIES_ASTRONAUTZOMBIE_H
#include "Zombie.h"

class AstronautZombie : public Zombie {
    static const int INITIAL_HEALTH = 500;
    static const int INITIAL_MOVEMENT_DELAY = 1;
    static const int INITIAL_ATTACK_POWER = 20;
    constexpr static const float INITIAL_FIRING_RATE = 1;
    static const int BRAIN_NEEDED_TO_CREATE = 200;
public:
    explicit AstronautZombie();
    QString getPicturePath() const override;
    QString getName() override;
    QString getMovementPicturePath() const override;
    QString getStayPicturePath() const override;
protected:
    void move() override;
};

#endif //PLANETVSZOMBIES_ASTRONAUTZOMBIE_H
