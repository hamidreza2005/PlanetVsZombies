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
#include "core/mediaplayer.h"

class PlayGround : public Window {
    Q_OBJECT
public:
    explicit PlayGround(ClientSocket* clientSocket, QWidget *parent = nullptr);
    void play();
    void connectDataListener() override;
    void handleServerResponse(const QJsonObject &data) override;
    void connectConnectionLostListener();
    void disconnectConnectionLostListener();
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
    void checkCardStates();
    void handleLanded();

private:
    QGraphicsView* graphicsView;
    QGraphicsScene* scene;
    QGraphicsScene* cardsScene;
    QGraphicsView* cardView;
    QVector<Card*> cards;
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
    QTimer* cardStateTimer;
    int remainingSeconds;
    int brainCount;
    int sunCount;
    Card* selectedCard;
    QGraphicsPixmapItem* rotatingItem;

    void createCards();
    void setupPlayerZombieInfo();
    void setupPlayerPlantInfo();
    void setupGround();
    void setupLayout();
    void setupRemainingTimeInfo();
    void setPlayerName();
    bool isPositionOccupied(QPointF point);
    void setupRotatingImage(const QString& imagePath);
    void endTheGame();
    void sendAddRequest(const QString& name,int x,int y);
    void addNewEntityFromServer(const QJsonObject &entityData);
    void cleanThePlayground();
    void startARound();
    void removeAllCards();
    void sendOverSocket(const QJsonObject& response);
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
