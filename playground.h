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
protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
    static QVector<std::function<GameEntity*()>> zombies;
    static QVector<std::function<GameEntity*()>> plants;
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
    Card* selectedCard;

    void createZombieCards();
    void createPlantCards();
    void setupPlayerZombieInfo();
    void setupPlayerPlantInfo();
    void setupGround();
    void setupLayout();
    void createGridLabels();
    void placeEntityAt(int row, int col);
public slots:
    void onCreateEntity(Card* card);
    void handleSceneClick(QPointF point);

};

#endif // PLAYGROUND_H
