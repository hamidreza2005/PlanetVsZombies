#include <QJsonArray>
#include "GameController.h"
#include "../bootstrap.h"
#include "../Player.h"
#include "../Cache.h"
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

    QJsonObject response;
    response["onlineUsers"] = ipAddresses;
    socket->writeOk(response);
}

void GameController::ready(TcpSocket *socket, const QJsonObject &request) {
    auto allClients =  Bootstrap::getInstance()->getClients();
    auto it = std::find_if(allClients.begin(), allClients.end(),[request,socket](auto clientSocket){
        QString ip = clientSocket->peerAddress().toString().split(":").last();
        return  ip == request["opponentIp"].toString() && clientSocket != socket->getOriginalSocket();
    });

    if (it == allClients.end()) {
        socket->sendValidationError("ip","Opponent either offline or does not exists");
        return;
    }
    auto opponentSocket = new TcpSocket(*it);
    auto firstPlayer = new Player(request["username"].toString(),socket);
    Cache::getInstance()->firstPlayer = firstPlayer;
    QJsonObject response;
    response["state"] = "getReady";
    response["username"] = request["username"];
    opponentSocket->write(response);
}

void GameController::verifyBeingReady(TcpSocket *socket, const QJsonObject &request) {
    if (Cache::getInstance()->firstPlayer == nullptr){
        socket->sendValidationError("game","there is no first player to start the game");
        return;
    }
    auto secondPlayer = new Player(request["username"].toString(),socket);
    Cache::getInstance()->secondPlayer = secondPlayer;
    srand(time(0));

    QString firstPlayerRole = rand() % 2 == 0 ? "zombie" : "plant";
    QString secondPlayerRole = firstPlayerRole == "zombie" ? "plant" : "zombie" ;

    QJsonObject response;
    response["state"] = "startTheGame";
    response["role"] = firstPlayerRole;
    Cache::getInstance()->firstPlayer->setRole(firstPlayerRole);
    Cache::getInstance()->firstPlayer->socket->write(response);

    response["role"] = secondPlayerRole;
    Cache::getInstance()->secondPlayer->setRole(secondPlayerRole);
    Cache::getInstance()->secondPlayer->socket->write(response);
}

void GameController::gameRoom(TcpSocket *socket, const QJsonObject &request) {
    if (!Cache::getInstance()->isThereAGamePlaying()){
        socket->sendValidationError("game","there is no game");
        return;
    }


}
