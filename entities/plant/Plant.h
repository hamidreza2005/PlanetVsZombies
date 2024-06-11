#ifndef PLANETVSZOMBIES_PLANT_H
#define PLANETVSZOMBIES_PLANT_H

#include "../GameEntity.h"
#include "QTimer"

class Plant : public GameEntity
{
public:
    explicit Plant(int health,int sun,float firingRate,int attackPower);

protected slots:
    void fire();
protected:
    int sun;
    float firingRate;
    int attackPower;
    QTimer *fireTimer;
private:
    void setUpTimers();
};


#endif
