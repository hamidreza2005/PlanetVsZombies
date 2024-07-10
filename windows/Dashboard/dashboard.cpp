#include <QJsonArray>
#include <QMessageBox>
#include "dashboard.h"
#include "ui_Dashboard.h"
#include "../../core/Cookie.h"

Dashboard::Dashboard(ClientSocket* clientSocket,QWidget *parent) :
        Window(clientSocket,parent), ui(new Ui::Dashboard) {
    ui->setupUi(this);
    this->updateOnlineUsersTimer = new QTimer();
    this->updateOnlineUsersTimer->setInterval(5000);
    connect(updateOnlineUsersTimer, &QTimer::timeout, this, [this](){
        this->getOnlineUsers();
    });
    connect(ui->onlineClients, &QListWidget::itemClicked, this, &Dashboard::onOnlineUserClicked);
}

Dashboard::~Dashboard() {
    delete ui;
    this->updateOnlineUsersTimer->stop();
    delete updateOnlineUsersTimer;
}

void Dashboard::handleServerResponse(const QJsonObject &data) {
    if (data["status"] == "419"){
        Window::showValidationErrors(this,data["errors"].toObject());
        return;
    }

    if (data.contains("onlineUsers")){
        QJsonArray onlineUsers = data["onlineUsers"].toArray();
        this->updateOnlineUsersBox(onlineUsers);
        return;
    }

    if (data.contains("state") && data.value("state") == "getReady"){
        this->verifyCurrentClientIsReadyToPlay(data.value("username").toString());
        return;
    }
    if (data.contains("state") && data.value("state") == "startTheGame"){
        Cookie::getInstance()->playingRound = data.value("round").toString();
        Cookie::getInstance()->opponentUsername = data.value("opponent").toString();
        Cookie::getInstance()->loggedInPlayer->getRole() = data.value("role").toString();
        emit this->startTheGame(this);
        return;
    }
}

void Dashboard::connectDataListener() {
    dataListener = connect(socket, &ClientSocket::dataReceived, this, &Dashboard::handleServerResponse);
    this->getOnlineUsers();
    ui->welcomeText->setText("Hi " + Cookie::getInstance()->loggedInPlayer->getUsername());
//    this->updateOnlineUsersTimer->start();
}

void Dashboard::getOnlineUsers() {
    try{
        this->socket->sendJson("getOnlineUsers",QJsonObject());
    }catch (...){

    }
}

void Dashboard::on_ready_clicked() {
    QString ip = ui->opponentIp->text();
    if(ip.isEmpty()){
        return;
    }
    try{
        this->socket->sendJson("ready",QJsonObject({{"opponentIp",ip},{"username",Cookie::getInstance()->loggedInPlayer->getUsername()}}));
    }catch (...){
        Window::showConnectionLostError(this);
    }
}

void Dashboard::updateOnlineUsersBox(const QJsonArray &users) {
    ui->onlineClients->clear(); // Clear the existing items
    int onlineUsersCount = users.size();
    ui->onlineClientsCount->setText(QString::number(onlineUsersCount));
    for (const QJsonValue &onlineUser : users) {
        QListWidgetItem *item = new QListWidgetItem(onlineUser.toString());
        ui->onlineClients->addItem(item);
    }
}

void Dashboard::onOnlineUserClicked(QListWidgetItem *item){
    QString ip = item->text();
    ui->opponentIp->setText(ip);
}

void Dashboard::disconnectDataListener() {
    Window::disconnectDataListener();
    this->updateOnlineUsersTimer->stop();
}

void Dashboard::on_exit_clicked() {
    Cookie::getInstance()->loggedInPlayer = nullptr;
    emit this->goToLoginPage(this);
}

void Dashboard::verifyCurrentClientIsReadyToPlay(const QString & opponentUsername) {
    QMessageBox::StandardButton reply;
    QString message = opponentUsername + " Wants to play with you. Do you want to do it?";
    reply = QMessageBox::question(this, "Ready Check", message,
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes){
        try{
            this->socket->sendJson("verifyReady", {{"username",Cookie::getInstance()->loggedInPlayer->getUsername()}});
        }catch (...){
            Window::showConnectionLostError(this);
        }
    }
}

void Dashboard::on_refreshUsers_clicked() {
    this->getOnlineUsers();
}

void Dashboard::on_history_clicked() {
    emit this->goToHistoryPage(this);
}

void Dashboard::on_updateCredentials_clicked() {
    emit this->goToCredentialsPage(this);
}
