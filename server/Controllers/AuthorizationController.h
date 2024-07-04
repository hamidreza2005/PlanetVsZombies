#ifndef SERVER_AUTHORIZATIONCONTROLLER_H
#define SERVER_AUTHORIZATIONCONTROLLER_H
#include "../TcpSocket.h"
#include <QJsonObject>


class AuthorizationController {
public:
    static void login(TcpSocket* socket,const QJsonObject& request);
    static void registration(TcpSocket* socket,const QJsonObject& request);
};


#endif //SERVER_AUTHORIZATIONCONTROLLER_H
