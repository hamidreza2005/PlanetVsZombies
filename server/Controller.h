#ifndef SERVER_CONTROLLER_H
#define SERVER_CONTROLLER_H
#include "QMap"
#include "QTcpSocket"
#include "QVector"

class Controller {
    static QMap<QString,std::function<void(QTcpSocket*,QVector<QString>&)>> *routes;
public:
    Controller();

    static void addRoute(const QString &name,std::function<void(QTcpSocket*,QVector<QString>&)> action);
    static bool hasRoute(const QString &name);
    static std::function<void(QTcpSocket*,QVector<QString>&)> getAction(const QString &name);

    void initializeRoutes();
};


#endif //SERVER_CONTROLLER_H
