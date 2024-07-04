#include <QJsonObject>
#include "Controller.h"

QMap<QString, std::function<void(TcpSocket*,const QJsonObject&)>> *Controller::routes = new QMap<QString,std::function<void(TcpSocket*,const QJsonObject&)>>;

Controller::Controller() {

}

void Controller::addRoute(const QString &name, const std::function<void(TcpSocket*,const QJsonObject&)>& action) {
    routes->insert(name,action);
}

bool Controller::hasRoute(const QString &name) {
    return routes->contains(name);
}

std::function<void(TcpSocket*,const QJsonObject&)> Controller::getAction(const QString &name) {
    return routes->value(name);
}

void Controller::initializeRoutes() {
    Controller::addRoute("login",[](TcpSocket* socket,const QJsonObject& list){
        socket->write("hi");
    });

    Controller::addRoute("register",[](TcpSocket* socket,const QJsonObject& list){
        QJsonObject response;
        response["status"] = "ok";
        socket->write(response);
    });
}
