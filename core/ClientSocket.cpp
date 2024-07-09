#include <QJsonDocument>
#include <QMessageBox>
#include "ClientSocket.h"
#include "exceptions/ConnectionIsLostException.h"
#include "../playground.h"

int ClientSocket::PORT;
QString ClientSocket::HOST;

ClientSocket::ClientSocket() {
}

ClientSocket::~ClientSocket() {
    if (socket){
        delete socket;
    }
    this->reconnectTimer->stop();
    delete this->reconnectTimer;
}

void ClientSocket::startSocket() {
    if(socket){
        delete socket;
    }
    socket = new QTcpSocket();
    socket->connectToHost(HOST, PORT);
}

void ClientSocket::socket_connected() {
    this->reconnectTimer->stop();
    qDebug() << "Socket is connected" ;
}

void ClientSocket::socket_disconnected() {
    emit this->connectionLost();
    qDebug() << "Socket got disconnected" ;
    if (!this->reconnectTimer->isActive()){
        this->reconnectTimer->start();
    }
}

void ClientSocket::read() {
    QByteArray data = this->socket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    emit dataReceived(doc.object());
}


void ClientSocket::sendJson(const QString& route,const QJsonObject &json) {
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    QString msg = route + "||"+ QString::fromUtf8(data);
    this->sendString(msg);
}

void ClientSocket::sendString(const QString &string) {
    if (!this->isConnected()){
        throw ConnectionIsLostException();
    }
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

void ClientSocket::connectToHost() {
    this->startSocket();
    this->connectSiganlsToListeners();
    this->reconnectTimer = new QTimer();
    this->reconnectTimer->setInterval(5000);
    connect(reconnectTimer, &QTimer::timeout, this, [this](){
        qDebug() << "trying to connect to host one more time";
        this->socket->connectToHost(HOST, PORT);
    });

    this->reconnectTimer->start();
}

void ClientSocket::setHost(const QString &ip, int port) {
    ClientSocket::HOST = ip;
    ClientSocket::PORT = port;
}
