#include <QMessageBox>
#include "login.h"
#include "ui_Login.h"
#include "../Window.h"
#include "../../playground.h"


Login::Login(ClientSocket* clientSocket,QWidget *parent) :
        Window(clientSocket,parent), ui(new Ui::Login) {
    ui->setupUi(this);
}

Login::~Login() {
    delete ui;
    delete socket;
}

void Login::on_submit_clicked() {
    dataListener = connect(socket, &ClientSocket::dataReceived, this, &Login::handleServerResponse);
    if(this->fieldsAreNotEmpty()){
        QMessageBox::information(this,"Error","Please Fill out all the fields");
        return;
    }

    if (!this->socket->isConnected()){
        QMessageBox::information(this,"Error","Connection is not possible right now.");
        return;
    }

    QJsonObject requestData;
    requestData["password"] = ui->password->text();
    requestData["username"] = ui->username->text();
    this->socket->sendJson("login",requestData);
}

void Login::handleServerResponse(const QJsonObject &data) {
    if (data["status"] == "200"){
        QMessageBox::information(this,"Success","your login was successfull",QMessageBox::Ok);
        emit this->goToDashboardPage(this);
        return;
    }
    Window::showValidationErrors(this,data["errors"].toObject());
}

bool Login::fieldsAreNotEmpty() {
    QVector<QString> fields = {
            ui->password->text(),
            ui->username->text(),
    };

    for(auto &i:fields){
        if (i.isEmpty()){
            return true;
        }
    }
    return false;
}

void Login::on_registerLink_clicked() {
    goToRegisterPage(this);
}

void Login::disconnectDataListener() {
    disconnect(dataListener);
}