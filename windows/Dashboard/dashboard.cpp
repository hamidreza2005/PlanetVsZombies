#include "dashboard.h"
#include "ui_Dashboard.h"


Dashboard::Dashboard(ClientSocket* clientSocket,QWidget *parent) :
        Window(clientSocket,parent), ui(new Ui::Dashboard) {
    ui->setupUi(this);
}

Dashboard::~Dashboard() {
    delete ui;
}

void Dashboard::handleServerResponse(const QJsonObject &data) {

}

void Dashboard::disconnectDataListener() {
    disconnect(dataListener);
}

void Dashboard::connectDataReceiver() {
    dataListener = connect(socket, &ClientSocket::dataReceived, this, &Dashboard::handleServerResponse);
}
