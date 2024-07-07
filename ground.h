#ifndef GROUND_H
#define GROUND_H

#include <QObject>
#include <QGraphicsPixmapItem>

class Ground : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Ground(QObject *parent = nullptr);
    static double getYForNewEntity(int y);
    static double getXForPlants(int x);
};

#endif // GROUND_H
