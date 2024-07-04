#include "TcpSocket.h"

TcpSocket::TcpSocket(QTcpSocket* socket):socket(socket) {

}

void TcpSocket::write(const QString& data){
    this->socket->write(data.toUtf8());
    this->socket->flush();
    socket->waitForBytesWritten(3000);
}