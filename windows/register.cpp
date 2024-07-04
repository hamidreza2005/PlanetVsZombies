#include "register.h"
#include "ui_Register.h"
#include "QMessageBox"

Register::Register(ClientSocket* clientSocket,QWidget *parent) :
        QWidget(parent), ui(new Ui::Register),socket(clientSocket) {
    ui->setupUi(this);
    dataListener = connect(clientSocket, &ClientSocket::dataReceived, this, &Register::handleServerResponse);
}

Register::~Register() {
    delete ui;
    delete socket;
    disconnect(dataListener);
}

void Register::on_submit_clicked() {
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
    this->socket->sendJson("register",requestData);
}

void Register::handleServerResponse(const QJsonObject &data) {
    if (data["status"] == "200"){
        QMessageBox::information(this,"Success","your registration was successfull",QMessageBox::Ok);
        exit(0);
    }
    this->showErrors(data["errors"].toObject());
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

void Register::showErrors(const QJsonObject &errors) {
    QString message = "";
    for(auto it = errors.constBegin();it != errors.constEnd();it++){
        message += it.key() + " : " + it.value()[0].toString() + "\n";
    }
    QMessageBox::critical(this,"Error",message);
}
