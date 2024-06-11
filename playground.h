#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QProgressBar>
#include <QTimer>
#include <QVector>
#include "Card.h"
#include "ground.h"

class PlayGround : public QWidget
{
    Q_OBJECT
public:
    explicit PlayGround(QWidget *parent = nullptr);
private slots:
    void updateTimer();
    void updateBrainCount(int amount);
    void updateSunCount(int amount);

private:
    QGraphicsView* graphicsView;
    QGraphicsScene* scene;
    QVector<Card*> zombieCards;
    QVector<Card*> plantCards;
    QLabel* playerZombieName;
    QLabel* playerPlantName;
    QLabel* remainingZombieTime;
    QLabel* remainingPlantTime;
    Ground* ground;
    QProgressBar* brainBar;
    QProgressBar* sunBar;
    bool isZombie;
    QTimer* timer;
    int remainingSeconds;
    int brainCount;
    int sunCount;

    void createZombieCards();
    void createPlantCards();
    void setupPlayerZombieInfo();
    void setupPlayerPlantInfo();
    void setupGround();
    void setupLayout();

public slots:
    void onCreateEntity(Card* card);
};

#endif
