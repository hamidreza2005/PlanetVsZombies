#ifndef SERVER_GAMECONTROLLER_H
#define SERVER_GAMECONTROLLER_H


#include <QJsonObject>
#include "../TcpSocket.h"

class GameController {
public:
    static void getOnlineUsers(TcpSocket* socket,const QJsonObject& request);
    static void verifyBeingReady(TcpSocket* socket,const QJsonObject& request);
    static void ready(TcpSocket* socket,const QJsonObject& request);
    static void gameRoom(TcpSocket* socket,const QJsonObject& request);
};


#endif //SERVER_GAMECONTROLLER_H
