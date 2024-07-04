#ifndef SERVER_CONTROLLER_H
#define SERVER_CONTROLLER_H
#include "QMap"
#include "QTcpSocket"
#include "QVector"
#include "../TcpSocket.h"
class Controller {
    static QMap<QString,std::function<void(TcpSocket*,const QJsonObject&)>> *routes;
public:
    Controller() = default;

    static void addRoute(const QString &name,const std::function<void(TcpSocket*,const QJsonObject&)>& action);
    static bool hasRoute(const QString &name);
    static std::function<void(TcpSocket*,const QJsonObject&)> getAction(const QString &name);

    static void initializeRoutes();
};


#endif //SERVER_CONTROLLER_H
