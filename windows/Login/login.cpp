#include <QMessageBox>
#include "login.h"
#include "ui_Login.h"
#include "../window.h"
#include "../../playground.h"
#include "../../core/Cookie.h"

Login::Login(ClientSocket* clientSocket,QWidget *parent) :
        Window(clientSocket,parent), ui(new Ui::Login) {
    ui->setupUi(this);
}

Login::~Login() {
    delete ui;
    delete socket;
}

void Login::on_submit_clicked() {
    if(this->fieldsAreNotEmpty()){
        QMessageBox::information(this,"Error","Please Fill out all the fields");
        return;
    }

    QJsonObject requestData;
    requestData["password"] = ui->password->text();
    requestData["username"] = ui->username->text();
    try{
        this->socket->sendJson("login",requestData);
    }catch (...){
        Window::showConnectionLostError(this);
    }
}

void Login::handleServerResponse(const QJsonObject &data) {
    if (data["status"] == "200"){
        Cookie::getInstance()->loggedInPlayer = new Player(data["user"]["username"].toString());
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
    emit goToRegisterPage(this);
}

void Login::connectDataListener() {
    dataListener = connect(socket, &ClientSocket::dataReceived, this, &Login::handleServerResponse);
}

void Login::on_resetPassword_clicked() {
    emit goToResetPasswordPage(this);
}