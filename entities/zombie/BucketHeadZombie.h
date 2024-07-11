#ifndef PLANETVSZOMBIES_BUCKETHEADZOMBIE_H
#define PLANETVSZOMBIES_BUCKETHEADZOMBIE_H

#include "Zombie.h"

class BucketHeadZombie : public Zombie {
    static const int INITIAL_HEALTH = 1950;
    static const int INITIAL_MOVEMENT_DELAY = 2;
    static const int INITIAL_ATTACK_POWER = 50;
    static const int INITIAL_FIRING_RATE = 1;
    static const int BRAIN_NEEDED_TO_CREATE = 200;
public:
    explicit BucketHeadZombie();
    QString getPicturePath() const override;
    QString getName() override;
    QString getMovementPicturePath() const override;
    QString getStayPicturePath() const override;
    QString getCardPicturePath() const override;

};

#endif //PLANETVSZOMBIES_BUCKETHEADZOMBIE_H
