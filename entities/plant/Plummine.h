#ifndef PLANETVSZOMBIES_PLUMMINE_H
#define PLANETVSZOMBIES_PLUMMINE_H

#include "Plant.h"

class PlumMine : public Plant
{
    static const int INITIAL_HEALTH = 0;
    static const int INITIAL_ATTACK_POWER = 500;
    static const int INITIAL_FIRING_RATE = 0;

public:
    explicit PlumMine();
    QString getPicturePath() const override;
    QString getName() override;
    void fire() override;
};

#endif // PLANETVSZOMBIES_PLUMMINE_H
