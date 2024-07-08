#ifndef SERVER_GAMECONTROLLER_H
#define SERVER_GAMECONTROLLER_H


#include <QJsonObject>
#include "../TcpSocket.h"
#include "../Player.h"

class GameController {
private:
    static bool isPlayerAllowedToAddEntity(bool isSentByFirstPlayer,Player* player1,Player* player2,QString entityName);
    static void handleEndOfTheGame(const QString& winnerRole);
public:
    static void getOnlineUsers(TcpSocket* socket,const QJsonObject& request);
    static void verifyBeingReady(TcpSocket* socket,const QJsonObject& request);
    static void ready(TcpSocket* socket,const QJsonObject& request);
    static void gameRoom(TcpSocket* socket,const QJsonObject& request);
};


#endif //SERVER_GAMECONTROLLER_H
