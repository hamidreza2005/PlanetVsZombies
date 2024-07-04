#ifndef PLANETVSZOMBIES_PEASHOOTER_H
#define PLANETVSZOMBIES_PEASHOOTER_H

#include "Plant.h"

class PeaShooter : public Plant
{
    static const int INITIAL_HEALTH = 200;
    static const int INITIAL_ATTACK_POWER = 15;
    static const int INITIAL_FIRING_RATE = 1;
public:
    explicit PeaShooter();
    QString getPicturePath() const override;
    QString getName() override;
};


#endif //PLANETVSZOMBIES_PEASHOOTER_H