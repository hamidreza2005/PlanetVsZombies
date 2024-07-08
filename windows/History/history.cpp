#include <QJsonArray>
#include "history.h"
#include "ui_History.h"
#include "../../core/Cookie.h"


History::History(ClientSocket* clientSocket,QWidget *parent) :
        Window(clientSocket,parent), ui(new Ui::History) {
    ui->setupUi(this);
}

History::~History() {
    delete ui;
}

void History::connectDataListener() {
    dataListener = connect(socket, &ClientSocket::dataReceived, this, &History::handleServerResponse);
    this->getHistoryFromServer();
}

void History::handleServerResponse(const QJsonObject &data) {
    if(data.value("status") == "200"){
        QJsonArray historyArray = data["history"].toArray();
        ui->table->setRowCount(historyArray.size());

        for (int i = 0; i < historyArray.size(); ++i) {
            QJsonObject historyItem = historyArray[i].toObject();
            QString firstRoundWinner = historyItem["firstRoundWinner"].toString();
            QString secondRoundWinner = historyItem["secondRoundWinner"].toString();
            QString winner = firstRoundWinner == secondRoundWinner ? firstRoundWinner : "DRAW";
            ui->table->setItem(i, 0, new QTableWidgetItem(historyItem["firstPlayer"].toString()));
            ui->table->setItem(i, 1, new QTableWidgetItem(historyItem["secondPlayer"].toString()));
            ui->table->setItem(i, 2, new QTableWidgetItem(firstRoundWinner));
            ui->table->setItem(i, 3, new QTableWidgetItem(secondRoundWinner));
            ui->table->setItem(i, 4, new QTableWidgetItem(winner));
        }
        return;
    }
    Window::showValidationErrors(this,data["erros"].toObject());
}

void History::on_dashboardLink_clicked() {
    emit this->goToDashboard(this);
}

void History::getHistoryFromServer() {
    try{
        this->socket->sendJson("getHistory",QJsonObject({{"username",Cookie::getInstance()->loggedInPlayer->getUsername()}}));
    }catch (...){
        Window::showConnectionLostError(this);
    }
}
