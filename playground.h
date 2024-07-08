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
    explicit PlayGround(ClientSocket* clientSocket,QWidget *parent = nullptr);
    void play();
    void connectDataListener() override;
    void handleServerResponse(const QJsonObject &data) override;
protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
    static QMap<QString,std::function<GameEntity*()>> zombies;
    static QMap<QString,std::function<GameEntity*()>> plants;

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
    QLabel* playerName;
    QLabel* remainingTime;
    Ground* ground;
    QProgressBar* brainBar;
    QProgressBar* sunBar;
    QHBoxLayout* infoLayout;
    QVBoxLayout* mainLayout;
    QMetaObject::Connection connectionLostListener;
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
    void setupRemainingTimeInfo();
    void setPlayerName();
    bool isPositionOccupied(QPointF point);
    void endTheGame();
    void sendAddRequest(const QString& name,int x,int y);
    void addNewEntityFromServer(QJsonObject entityData);

public slots:
    void selectCard(Card* card);
    void addEntity(QPointF point);
    void connectionLost();
    void AZombieReachedTheEnd();
    void ranOutOfTime();
signals:
    void goToDashboardPage(Window* sender);
};

#endif // PLAYGROUND_H
