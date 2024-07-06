#include <QJsonArray>
#include "GameController.h"
#include "../bootstrap.h"
void GameController::getOnlineUsers(TcpSocket *socket, const QJsonObject &request) {
    auto allClients =  Bootstrap::getInstance()->getClients();
    auto it = std::find(allClients.begin(), allClients.end(),socket->getOriginalSocket());
    if (it != allClients.end()) {
        allClients.erase(it);
    }

    QJsonArray ipAddresses;

    for (auto &clientSocket : allClients) {
        if (clientSocket) {
            QString ipAddress = clientSocket->peerAddress().toString();
            ipAddress = ipAddress.split(":").last();
            ipAddresses.append(ipAddress);
        }
    }

    // Create a JSON object to send as the response
    QJsonObject response;
    response["onlineUsers"] = ipAddresses;
    socket->writeOk(response);
}

void GameController::ready(TcpSocket *socket, const QJsonObject &request) {
    auto allClients =  Bootstrap::getInstance()->getClients();
    auto it = std::find_if(allClients.begin(), allClients.end(),[request,socket](auto clientSocket){
        QString ip = clientSocket->peerAddress().toString().split(":").last();
//        qDebug() << clientSocket->objectName() << socket->objectName();
        return  ip == request["opponentIp"].toString() && clientSocket != socket->getOriginalSocket();
    });

    if (it == allClients.end()) {
        socket->sendValidationError("ip","Opponent either offline or does not exists");
        return;
    }

    auto opponentSocket = new TcpSocket(*it);
    QJsonObject response;
    response["state"] = "getReady";
    opponentSocket->write(response);
}
