#ifndef PLANETVSZOMBIES_BULLET_H
#define PLANETVSZOMBIES_BULLET_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include "../GameEntity.h"

class Bullet : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Bullet(int attackPower, int speed, QGraphicsItem* parent = nullptr);
    void setImage(const QString& imagePath);

public slots:
    void move();

private:
    int attackPower;
    int speed;
};

#endif // PLANETVSZOMBIES_BULLET_H
