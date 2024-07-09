#ifndef PLANETVSZOMBIES_BOOMERANG_H
#define PLANETVSZOMBIES_BOOMERANG_H

#include "Plant.h"
#include <QTimer>
#include <QStringList>

class Boomerang : public Plant {
    Q_OBJECT

    static const int INITIAL_HEALTH = 200;
    static const int INITIAL_ATTACK_POWER = 30;
    static const int INITIAL_FIRING_RATE = 1;
    static const int SUN_NEEDED_TO_CREATE = 125;

public:
    explicit Boomerang();
    QString getPicturePath() const override;
    QString getName() override;
    void fire() override;

signals:
    void imageChanged(const QString& newImagePath);

private slots:
    void changeBulletImage();

private:
    QTimer* imageChangeTimer;
    QStringList bulletImages;
    int currentImageIndex;
};

#endif // PLANETVSZOMBIES_BOOMERANG_H
