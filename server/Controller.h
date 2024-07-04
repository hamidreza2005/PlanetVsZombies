#ifndef SERVER_CONTROLLER_H
#define SERVER_CONTROLLER_H
#include "QMap"
#include "QTcpSocket"
#include "QVector"
#include "TcpSocket.h"
class Controller {
    static QMap<QString,std::function<void(TcpSocket*,QVector<QString>&)>> *routes;
public:
    Controller();

    static void addRoute(const QString &name,std::function<void(TcpSocket*,QVector<QString>&)> action);
    static bool hasRoute(const QString &name);
    static std::function<void(TcpSocket*,QVector<QString>&)> getAction(const QString &name);

    static void initializeRoutes();
};


#endif //SERVER_CONTROLLER_H
