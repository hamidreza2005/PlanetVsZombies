#ifndef PLANETVSZOMBIES_WALNUT_H
#define PLANETVSZOMBIES_WALNUT_H

#include "Plant.h"

class Walnut : public Plant
{
    static const int INITIAL_HEALTH = 400;
    static const int INITIAL_ATTACK_POWER = 0;
    static const int INITIAL_FIRING_RATE = 1;
    static const int SUN_NEEDED_TO_CREATE = 100;

public:
    explicit Walnut();
    QString getPicturePath() const override;
    QString getName() override;
    void fire() override;
};

#endif // PLANETVSZOMBIES_WALNUT_H
