#ifndef PLANETVSZOMBIES_REGULARZOMBIE_H
#define PLANETVSZOMBIES_REGULARZOMBIE_H
#include "Zombie.h"

class RegularZombie: public Zombie {
    static const int INITIAL_HEALTH = 500;
    static const int INITIAL_MOVEMENT_DELAY = 1;
    static const int INITIAL_ATTACK_POWER = 25;
    static const int INITIAL_FIRING_RATE = 1;
    static const int BRAIN_NEEDED_TO_CREATE = 100;

public:
    explicit RegularZombie();
    QString getPicturePath() const override;
    QString getName() override;
    QString getMovementPicturePath() const override;
    QString getStayPicturePath() const override;
};

#endif //PLANETVSZOMBIES_REGULARZOMBIE_H
