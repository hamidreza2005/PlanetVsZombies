#include <QJsonDocument>
#include "ClientSocket.h"

const int ClientSocket::PORT = 2000;
const QString ClientSocket::HOST = "127.0.0.1";

ClientSocket::ClientSocket() {
    this->connectToHost();
    this->connectSiganlsToListeners();
    this->reconnectTimer = new QTimer();
    this->reconnectTimer->setInterval(10000);
    connect(reconnectTimer, &QTimer::timeout, this, [this](){
        this->connectToHost();
    });
}

ClientSocket::~ClientSocket() {
    if (socket){
        delete socket;
    }
    this->reconnectTimer->stop();
    delete this->reconnectTimer;
}

void ClientSocket::connectToHost() {
    socket = new QTcpSocket();
    socket->connectToHost(HOST, PORT);
}

void ClientSocket::socket_connected() {
    qDebug() << "Socket is connected" ;
}

void ClientSocket::socket_disconnected() {
//    if (!this->reconnectTimer->isActive()){
//        this->reconnectTimer->start();
//    }
}

void ClientSocket::read() {
    QByteArray data = this->socket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    emit dataReceived(doc.object());
}


void ClientSocket::sendJson(const QString& route,const QJsonObject &json) {
    if(!this->isConnected()){
        this->connectToHost();
    }
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    QString msg = route + "||"+ QString::fromUtf8(data);
    this->sendString(msg);
}

void ClientSocket::sendString(const QString &string) {
    socket->write(string.toUtf8());
    socket->flush();
}

bool ClientSocket::isConnected() {
    return socket->state() == QTcpSocket::ConnectedState;
}

void ClientSocket::connectSiganlsToListeners() {
    connect(socket, &QTcpSocket::connected, this, &ClientSocket::socket_connected);
    connect(socket, &QTcpSocket::readyRead, this, &ClientSocket::read);
//    connect(socket, &QTcpSocket::bytesWritten, this, &MainWindow::socket_bytesWritten);
    connect(socket, &QTcpSocket::disconnected, this, &ClientSocket::socket_disconnected);
}
