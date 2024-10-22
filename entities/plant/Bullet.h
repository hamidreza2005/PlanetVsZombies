#ifndef PLANETVSZOMBIES_BULLET_H
#define PLANETVSZOMBIES_BULLET_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include "../GameEntity.h"
#include "../zombie/Zombie.h"
#include <QList>
class Bullet : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Bullet(int attackPower, int speed = 15,bool canHitMultipleZombies = false,int size = 20, QGraphicsItem* parent = nullptr);
    ~Bullet() override;

    void setImage(const QString& imagePath);
public slots:
    void move();

private:
    int attackPower;
    int speed;
    int size;
    bool canHitMultipleZombies;
    QTimer* movementTimer;
    QList<Zombie*>* hitZombies;
};

#endif // PLANETVSZOMBIES_BULLET_H
