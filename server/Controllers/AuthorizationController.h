#ifndef SERVER_AUTHORIZATIONCONTROLLER_H
#define SERVER_AUTHORIZATIONCONTROLLER_H
#include "../TcpSocket.h"
#include <QJsonObject>

class AuthorizationController {
private:
    static QJsonObject getUserDataToRegister(const QJsonObject &request);
    static QJsonObject userUniqueDataErrors(const QJsonObject &data);
    static QString hashString(const QString& str);
public:
    static void login(TcpSocket* socket,const QJsonObject& request);
    static void registration(TcpSocket* socket,const QJsonObject& request);
    static void resetPassword(TcpSocket* socket,const QJsonObject& request);
};


#endif //SERVER_AUTHORIZATIONCONTROLLER_H
