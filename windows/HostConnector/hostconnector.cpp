#include <QMessageBox>
#include "hostconnector.h"
#include "ui_hostconnector.h"


HostConnector::HostConnector(ClientSocket* clientSocket,QWidget *parent) :
        Window(clientSocket,parent), ui(new Ui::HostConnector) {
    ui->setupUi(this);
}

HostConnector::~HostConnector() {
    delete ui;
}

void HostConnector::connectDataListener() {
    dataListener = connect(socket, &ClientSocket::dataReceived, this, &HostConnector::handleServerResponse);
}

void HostConnector::handleServerResponse(const QJsonObject &data) {
//    if (data["status"] == "200"){
//        Cookie::getInstance()->loggedInPlayer = new Player(data["user"]["username"].toString());
//        emit this->goToDashboardPage(this);
//        return;
//    }
//    Window::showValidationErrors(this,data["errors"].toObject());
}

void HostConnector::on_connect_clicked() {
    if(ui->ip->text() == "" || ui->port->value() == 0){
        QMessageBox::information(this,"Problem","Please Fill out all the inputs");
        return;
    }
    ClientSocket::setHost(ui->ip->text(),ui->port->value());
    this->socket->connectToHost();
    emit this->connectionIsEstablished(this);
}
