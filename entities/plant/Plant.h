#ifndef PLANETVSZOMBIES_PLANT_H
#define PLANETVSZOMBIES_PLANT_H

#include "../GameEntity.h"
#include <QTimer>

class Plant : public GameEntity
{
public:
    explicit Plant(int health, int sun, float firingRate, int attackPower);
    virtual ~Plant();
    virtual void fire() = 0;
    [[nodiscard]] virtual QString getCardPicturePath() const override = 0;


protected slots:
    void fireImpl();

protected:
    int sun;
    float firingRate;
    int attackPower;
    QTimer *fireTimer;
    virtual bool isThereAZombieInTheRow();
private:
    void setUpTimers();
};

#endif // PLANETVSZOMBIES_PLANT_H
