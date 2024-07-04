#include <QJsonDocument>
#include <QJsonObject>
#include "TcpSocket.h"

TcpSocket::TcpSocket(QTcpSocket* socket):socket(socket) {

}

void TcpSocket::write(const QString& data){
    this->socket->write(data.toUtf8());
    this->socket->flush();
    socket->waitForBytesWritten(3000);
}

void TcpSocket::write(QJsonObject &data) {
    QJsonDocument responseDoc(data);
    socket->write(responseDoc.toJson());
    socket->flush();
    socket->waitForBytesWritten(3000);
}

void TcpSocket::sendValidationError(QJsonValue errors) {
    QJsonObject response;
    response["errors"] = errors;
    response["status"] = 419;
    this->write(response);
}
