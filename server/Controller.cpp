#include "Controller.h"

QMap<QString, std::function<void(TcpSocket*,QVector<QString>&)>> *Controller::routes = new QMap<QString,std::function<void(TcpSocket*,QVector<QString>&)>>;

Controller::Controller() {

}

void Controller::addRoute(const QString &name, std::function<void(TcpSocket*,QVector<QString>&)> action) {
    routes->insert(name,action);
}

bool Controller::hasRoute(const QString &name) {
    return routes->contains(name);
}

std::function<void(TcpSocket*,QVector<QString>&)> Controller::getAction(const QString &name) {
    return routes->value(name);
}

void Controller::initializeRoutes() {
    Controller::addRoute("login",[](TcpSocket* socket,QVector<QString>& list){
        socket->write("hi");
    });

    Controller::addRoute("register",[](TcpSocket* socket,QVector<QString>& list){
        socket->write("you are correct");
    });
}
