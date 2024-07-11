#ifndef PLAYGROUND_H
#define PLAYGROUND_H
#define INITIAL_POINTS 0
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
#include "../Window.h"
#include "../../core/ClientSocket.h"
#include "Chat.h"

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
    static QMap<QString, std::function<GameEntity*()>> zombies;
    static QMap<QString, std::function<GameEntity*()>> plants;

private slots:
    void updateTimer();
    void updatePoint(int amount);
    void spawnSunBrain();
    void collectSunBrain(int value);
    void checkCardStates();
    void handleLanded();
    void sendMessageToOpponent(const QString &message);
    void playerWantsToResign();
private:
    QGraphicsView* graphicsView;
    QGraphicsScene* scene;
    QGraphicsScene* cardsScene;
    QGraphicsView* cardView;
    QVector<Card*> cards;
    QLabel* playerName;
    QLabel* remainingTime;
    Ground* ground;
    QProgressBar* pointsBar;
    QHBoxLayout* infoLayout;
    QVBoxLayout* mainLayout;
    QVBoxLayout* chatLayout;
    QHBoxLayout* containerLayout;
    QMetaObject::Connection connectionLostListener;
    bool isZombie;
    QTimer* timer;
    QTimer* sunBrainTimer;
    int remainingSeconds;
    int points;
    Card* selectedCard;
    QGraphicsPixmapItem* rotatingItem;
    Chat* chatHandler;

    void setUpChatBox();
    void createCards();
    void setUpPointsBar();
    void setupGround();
    void setupLayout();
    void setupRemainingTimeInfo();
    void setPlayerName();
    bool isPositionOccupied(QPointF point);
    void setupRotatingImage(const QString& imagePath);
    void endTheGame();
    void sendAddRequest(const QString& name, int x, int y);
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
