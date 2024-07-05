#include <QMessageBox>
#include "resetpassword.h"
#include "ui_ResetPassword.h"


ResetPassword::ResetPassword(ClientSocket* clientSocket,QWidget *parent) :
        Window(clientSocket,parent), ui(new Ui::ResetPassword) {
    ui->setupUi(this);
}

ResetPassword::~ResetPassword() {
    delete ui;
}

void ResetPassword::handleServerResponse(const QJsonObject &data) {
    if (data["status"] == "200"){
        QMessageBox::information(this,"Success","your password reset Successfully",QMessageBox::Ok);
        emit this->goToLoginPage(this);
        return;
    }
    Window::showValidationErrors(this,data["errors"].toObject());
}

void ResetPassword::connectDataListener() {
    dataListener = connect(socket, &ClientSocket::dataReceived, this, &ResetPassword::handleServerResponse);
}

void ResetPassword::on_loginLink_clicked() {
    emit this->goToLoginPage(this);
}

void ResetPassword::on_submit_clicked() {
    this->connectDataListener();
    if(this->fieldsAreNotEmpty()){
        QMessageBox::information(this,"Error","Please Fill out all the fields");
        return;
    }

    if (!this->socket->isConnected()){
        QMessageBox::information(this,"Error","Connection is not possible right now.");
        return;
    }

    QJsonObject requestData;
    requestData["password"] = ui->newPassword->text();
    requestData["phone"] = ui->phoneNumber->text();
    this->socket->sendJson("resetPassword",requestData);
}

bool ResetPassword::fieldsAreNotEmpty() {
    QVector<QString> fields = {
            ui->newPassword->text(),
            ui->phoneNumber->text(),
    };

    for(auto &i:fields){
        if (i.isEmpty()){
            return true;
        }
    }
    return false;
}
