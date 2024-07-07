#ifndef PLANETVSZOMBIES_BULLET_H
#define PLANETVSZOMBIES_BULLET_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include "../GameEntity.h"

class Bullet : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Bullet(int attackPower, int speed,bool canHitMultipleZombies = false, QGraphicsItem* parent = nullptr);
    ~Bullet() override;

public slots:
    void move();

private:
    int attackPower;
    int speed;
    bool canHitMultipleZombies;
    QTimer* movementTimer;

    void setImage(const QString& imagePath);
};

#endif // PLANETVSZOMBIES_BULLET_H
