#ifndef PLANETVSZOMBIES_JALAPENO_H
#define PLANETVSZOMBIES_JALAPENO_H

#include "Plant.h"

class Jalapeno : public Plant
{
    static const int INITIAL_HEALTH = 0;
    static const int INITIAL_ATTACK_POWER = 300;
    static const int INITIAL_FIRING_RATE = 1;
    static const int SUN_NEEDED_TO_CREATE = 150;

public:
    explicit Jalapeno();
    QString getPicturePath() const override;
    QString getName() override;
    void fire() override;
protected:
    bool isThereAZombieInTheRow() override;
};

#endif // PLANETVSZOMBIES_JALAPENO_H
