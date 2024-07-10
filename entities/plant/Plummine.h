#ifndef PLANETVSZOMBIES_PLUMMINE_H
#define PLANETVSZOMBIES_PLUMMINE_H

#include "Plant.h"

class PlumMine : public Plant
{
    static const int INITIAL_HEALTH = 0;
    static const int INITIAL_ATTACK_POWER = 500;
    static const int INITIAL_FIRING_RATE = 1;
    static const int SUN_NEEDED_TO_CREATE = 175;

public:
    explicit PlumMine();
    QString getPicturePath() const override;
    QString getName() override;
    void fire() override;
protected:
    bool isThereAZombieInTheRow() override;
};

#endif // PLANETVSZOMBIES_PLUMMINE_H
