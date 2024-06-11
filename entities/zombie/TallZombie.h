#ifndef PLANETVSZOMBIES_TALLZOMBIE_H
#define PLANETVSZOMBIES_TALLZOMBIE_H
#include "Zombie.h"

class TallZombie : public Zombie
{
    static const int INITIAL_HEALTH = 500;
    static const int INITIAL_MOVEMENT_DELAY = 1;
    static const int INITIAL_ATTACK_POWER = 30;
    static const int INITIAL_FIRING_RATE = 1;
    static const int BRAIN_NEEDED_TO_CREATE = 150;
public:
    explicit TallZombie();
    QString getPicturePath() const override;
    QString getName() override;
};


#endif //PLANETVSZOMBIES_TALLZOMBIE_H
