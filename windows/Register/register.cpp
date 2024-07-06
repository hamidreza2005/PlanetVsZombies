#include "register.h"
#include "ui_Register.h"
#include "QMessageBox"

Register::Register(ClientSocket* clientSocket,QWidget *parent) :
        Window(clientSocket,parent), ui(new Ui::Register) {
    ui->setupUi(this);
}

Register::~Register() {
    delete ui;
    delete socket;
    disconnect(dataListener);
}

void Register::on_submit_clicked() {
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
    requestData["name"] = ui->name->text();
    requestData["email"] = ui->email->text();
    requestData["password"] = ui->password->text();
    requestData["username"] = ui->username->text();
    requestData["phone"] = ui->phone->text();
    try{
        this->socket->sendJson("register",requestData);
    }catch (...){
        Window::showConnectionLostError(this);
    }
}

void Register::handleServerResponse(const QJsonObject &data) {
    if (data["status"] == "200"){
        QMessageBox::information(this,"Success","your registration was successfull",QMessageBox::Ok);
        emit this->goToLoginPage(this);
        return;
    }
    Window::showValidationErrors(this,data["errors"].toObject());
}

bool Register::fieldsAreNotEmpty() {
    QVector<QString> fields = {
            ui->name->text(),
            ui->email->text(),
            ui->password->text(),
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

void Register::on_loginLink_clicked() {
    this->goToLoginPage(this);
}

void Register::connectDataListener() {
    dataListener = connect(socket, &ClientSocket::dataReceived, this, &Register::handleServerResponse);
}