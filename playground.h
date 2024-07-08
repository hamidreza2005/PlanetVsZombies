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
#include "SunBrain.h"
#include "windows/Window.h"
#include "core/ClientSocket.h"

class PlayGround : public Window {
    Q_OBJECT
public:
    explicit PlayGround(ClientSocket* clientSocket, QWidget *parent = nullptr);
    void play();
    void connectDataListener() override;
    void handleServerResponse(const QJsonObject &data) override;
protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
    static QVector<std::function<GameEntity*()>> zombies;
    static QVector<std::function<GameEntity*()>> plants;

private slots:
    void updateTimer();
    void updateBrainCount(int amount);
    void updateSunCount(int amount);
    void spawnSunBrain();
    void collectSunBrain(int value);

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
    QHBoxLayout* infoLayout;
    QVBoxLayout* mainLayout;
    bool isZombie;
    QTimer* timer;
    QTimer* sunBrainTimer;
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
    bool isOutOfGround(const QPointF* point);
    bool isPositionOccupied(QPointF point);
    void setupRotatingImage(const QString& imagePath, int xPos, int yPos);
    void endTheGame();
public slots:
    void selectCard(Card* card);
    void addEntity(QPointF point);
    void connectionLost();
signals:
    void goToDashboardPage(Window* sender);
};

#endif // PLAYGROUND_H
