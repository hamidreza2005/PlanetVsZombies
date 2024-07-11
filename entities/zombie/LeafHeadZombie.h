#ifndef PLANETVSZOMBIES_LEAFHEADZOMBIE_H
#define PLANETVSZOMBIES_LEAFHEADZOMBIE_H
#include "Zombie.h"

class LeafHeadZombie : public Zombie {
    static const int INITIAL_HEALTH = 800;
    static const int INITIAL_MOVEMENT_DELAY = 1;
    static const int INITIAL_ATTACK_POWER = 25;
    constexpr static const float INITIAL_FIRING_RATE = 1;
    static const int BRAIN_NEEDED_TO_CREATE = 150;
public:
    explicit LeafHeadZombie();
    QString getPicturePath() const override;
    QString getName() override;
    QString getMovementPicturePath() const override;
    QString getStayPicturePath() const override;
    QString getCardPicturePath() const override;

};

#endif //PLANETVSZOMBIES_LEAFHEADZOMBIE_H
