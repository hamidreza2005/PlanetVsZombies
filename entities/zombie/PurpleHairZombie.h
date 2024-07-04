//
// Created by hrhas on 6/11/2024.
//

#ifndef PLANETVSZOMBIES_PURPLEHAIRZOMBIE_H
#define PLANETVSZOMBIES_PURPLEHAIRZOMBIE_H
#include "Zombie.h"

class PurpleHairZombie : public Zombie{
    static const int INITIAL_HEALTH = 4000;
    static const int INITIAL_MOVEMENT_DELAY = 1;
    static const int INITIAL_ATTACK_POWER = 75;
    constexpr static const float INITIAL_FIRING_RATE = 0.5;
    static const int BRAIN_NEEDED_TO_CREATE = 800;
public:
    explicit PurpleHairZombie();
    QString getPicturePath() const override;
    QString getName() override;
};


#endif //PLANETVSZOMBIES_PURPLEHAIRZOMBIE_H