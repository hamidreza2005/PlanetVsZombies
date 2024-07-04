#include "ClientSocket.h"

const int ClientSocket::PORT = 2000;
const QString ClientSocket::HOST = "127.0.0.1";

ClientSocket::ClientSocket() {
    this->connectToHost();
}

ClientSocket::~ClientSocket() {
    if (socket){
        delete socket;
    }
}

void ClientSocket::connectToHost() {
    socket = new QTcpSocket();
    socket->connectToHost(HOST, PORT);

    connect(socket, &QTcpSocket::connected, this, &ClientSocket::socket_connected);
    connect(socket, &QTcpSocket::readyRead, this, &ClientSocket::read);
//    connect(socket, &QTcpSocket::bytesWritten, this, &MainWindow::socket_bytesWritten);
    connect(socket, &QTcpSocket::disconnected, this, &ClientSocket::socket_disconnected);
}

void ClientSocket::socket_connected() {
    qDebug() << "Socket is connected" ;
}

void ClientSocket::socket_disconnected() {
    qDebug() << "We Lost the connection";
}

void ClientSocket::read() {
    QByteArray data = this->socket->readAll();
    qDebug() << data;
}
