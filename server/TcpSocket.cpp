#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
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

void TcpSocket::sendValidationError(QJsonValue errors,int statusCode) {
    QJsonObject response;
    response["errors"] = errors;
    response["status"] = statusCode;
    this->write(response);
}

void TcpSocket::sendValidationError(QString field, QString error, int statusCode) {
    QJsonObject errorBag;
    QJsonArray errorsArray;
    errorsArray.append(error);
    errorBag[field] = errorsArray;
    this->sendValidationError(QJsonValue(errorBag),statusCode);
}

QTcpSocket *TcpSocket::getOriginalSocket() {
    return this->socket;
}

void TcpSocket::writeOk(QJsonObject &data) {
    data["status"] = 200;
    this->write(data);
}

bool TcpSocket::isConnected() {
    return this->socket->state() == QTcpSocket::ConnectedState;
}
