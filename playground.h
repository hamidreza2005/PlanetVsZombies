#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QVector>
#include "Card.h"

class PlayGround : public QWidget
{
    Q_OBJECT
public:
    explicit PlayGround(QWidget *parent = nullptr);
private:
    QGraphicsView* graphicsView;
    QGraphicsScene* scene;
    QVector<Card*> zombieCards;
    QVector<Card*> plantCards;
    QLabel* playerZombieName;
    QLabel* playerPlantName;
    QLabel* brainCount;
    QLabel* sunCount;
    QLabel* remainingZombieTime;
    QLabel* remainingPlantTime;
    bool isZombie;

    void createZombieCards();
    void createPlantCards();
    void setupPlayerZombieInfo();
    void setupPlayerPlantInfo();
    void setupZombieGround();
    void setupPlantGround();
    void setupLayout();
};

#endif
