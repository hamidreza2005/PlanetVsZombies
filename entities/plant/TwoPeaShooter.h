#ifndef PLANETVSZOMBIES_TWOPEASHOOTER_H
#define PLANETVSZOMBIES_TWOPEASHOOTER_H

#include "Plant.h"

class TwoPeaShooter : public Plant
{
    static const int INITIAL_HEALTH = 200;
    static const int INITIAL_ATTACK_POWER = 40;
    static const int INITIAL_FIRING_RATE = 1;
    static const int SUN_NEEDED_TO_CREATE = 100;

public:
    explicit TwoPeaShooter();
    QString getPicturePath() const override;
    QString getName() override;
    void fire() override;
    QString getCardPicturePath() const override;

};

#endif // PLANETVSZOMBIES_TWOPEASHOOTER_H
