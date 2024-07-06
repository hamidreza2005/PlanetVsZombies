#ifndef PLANETVSZOMBIES_BOOMERANG_H
#define PLANETVSZOMBIES_BOOMERANG_H

#include "Plant.h"

class Boomerang : public Plant
{
    static const int INITIAL_HEALTH = 200;
    static const int INITIAL_ATTACK_POWER = 30;
    static const int INITIAL_FIRING_RATE = 1;

public:
    explicit Boomerang();
    QString getPicturePath() const override;
    QString getName() override;
    void fire() override;
};

#endif // PLANETVSZOMBIES_BOOMERANG_H
