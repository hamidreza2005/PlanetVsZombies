#include <QMessageBox>
#include "updatecredentials.h"
#include "ui_updatecredentials.h"
#include "../../core/Cookie.h"

UpdateCredentials::UpdateCredentials(ClientSocket* clientSocket,QWidget *parent) :
        Window(clientSocket,parent), ui(new Ui::UpdateCredentials) {
    ui->setupUi(this);
}

UpdateCredentials::~UpdateCredentials() {
    delete ui;
}

void UpdateCredentials::connectDataListener() {
    dataListener = connect(socket, &ClientSocket::dataReceived, this, &UpdateCredentials::handleServerResponse);
    this->getUserData();
}

void UpdateCredentials::handleServerResponse(const QJsonObject &data) {
    if (data["status"] == "200"){
        if(data.contains("currentUser")){
            this->fillInputsUsingUserData(data["currentUser"].toObject());
            return;
        }
        QMessageBox::information(this,"Success","You successfully updated your credentials",QMessageBox::Ok);
        Cookie::getInstance()->loggedInPlayer->setUsername(data["user"].toObject()["username"].toString());
        emit this->goToDashboardPage(this);
        return;
    }
    Window::showValidationErrors(this,data["errors"].toObject());
}

void UpdateCredentials::on_dashboardLink_clicked() {
    emit this->goToDashboardPage(this);
}

void UpdateCredentials::on_submit_clicked() {
    if(this->fieldsAreNotEmpty()){
        QMessageBox::information(this,"Error","Please Fill out all the fields");
        return;
    }

    if (!this->socket->isConnected()){
        QMessageBox::information(this,"Error","Connection is not possible right now.");
        return;
    }

    QJsonObject requestData;
    requestData["currentUsername"] = Cookie::getInstance()->loggedInPlayer->getUsername();
    requestData["name"] = ui->name->text();
    requestData["email"] = ui->email->text();
    requestData["password"] = ui->password->text();
    requestData["username"] = ui->username->text();
    requestData["phoneNumber"] = ui->phone->text();
    try{
        this->socket->sendJson("updateCredentials",requestData);
    }catch (...){
        Window::showConnectionLostError(this);
    }
}

bool UpdateCredentials::fieldsAreNotEmpty() {
    QVector<QString> fields = {
            ui->name->text(),
            ui->email->text(),
            ui->username->text(),
            ui->phone->text(),
    };

    for(auto &i:fields){
        if (i.isEmpty()){
            return true;
        }
    }
    return false;
}

void UpdateCredentials::getUserData() {
    QJsonObject requestData;
    requestData["username"] = Cookie::getInstance()->loggedInPlayer->getUsername();
    try{
        this->socket->sendJson("getUserData",requestData);
    }catch (...){
        Window::showConnectionLostError(this);
    }
}

void UpdateCredentials::fillInputsUsingUserData(const QJsonObject &data) {
    ui->name->setText(data["username"].toString());
    ui->email->setText(data["email"].toString());
    ui->username->setText(data["username"].toString());
    ui->phone->setText(data["phoneNumber"].toString());
}
