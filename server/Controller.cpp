#include "Controller.h"

QMap<QString, std::function<void(QTcpSocket*,QVector<QString>&)>> *Controller::routes = new QMap<QString,std::function<void(QTcpSocket*,QVector<QString>&)>>;

Controller::Controller() {

}

void Controller::addRoute(const QString &name, std::function<void(QTcpSocket*,QVector<QString>&)> action) {
    routes->insert(name,action);
}

bool Controller::hasRoute(const QString &name) {
    return routes->contains(name);
}

std::function<void(QTcpSocket*,QVector<QString>&)> Controller::getAction(const QString &name) {
    return routes->value(name);
}

void Controller::initializeRoutes() {
    Controller::addRoute("login",[](QTcpSocket* socket,QVector<QString>& list){
        socket->write("hi");
    });
}
