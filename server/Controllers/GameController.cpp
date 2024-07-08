#include <QJsonArray>
#include "GameController.h"
#include "../bootstrap.h"
#include "../Cache.h"
#include "../Database/DB.h"
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
    response["round"] = "1";
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
    Player* firstPlayer = Cache::getInstance()->firstPlayer;
    Player* secondPlayer = Cache::getInstance()->secondPlayer;
    bool isSentByFirstPlayer = socket->getOriginalSocket() == firstPlayer->socket->getOriginalSocket();

    if (request["state"] == "zombieReachedTheEnd"){
        GameController::handleEndOfTheGame("zombie");
        return;
    }

    if(request["state"] == "timeEnded" && isSentByFirstPlayer){
        GameController::handleEndOfTheGame("plant");
        return;
    }

    if (request["state"] == "add"){
//        if (GameController::isPlayerAllowedToAddEntity(isSentByFirstPlayer,firstPlayer,secondPlayer,request["entity"].toString())){
//          return;
//        }

        QJsonObject response = request;
        if(isSentByFirstPlayer){
            secondPlayer->socket->write(response);
        }else if(socket->getOriginalSocket() == secondPlayer->socket->getOriginalSocket()){
            firstPlayer->socket->write(response);
        }
        return;
    }
}

bool GameController::isPlayerAllowedToAddEntity(bool isSentByFirstPlayer,Player* player1,Player* player2,QString entityName) {
    return (isSentByFirstPlayer && player1->getRole() != entityName)
           || (!isSentByFirstPlayer && player2->getRole() != entityName);
}

void GameController::handleEndOfTheGame(const QString &winnerRole) {
    Player* firstPlayer = Cache::getInstance()->firstPlayer;
    Player* secondPlayer = Cache::getInstance()->secondPlayer;
    Player** roundWinner = Cache::getInstance()->firstRoundWinner == nullptr ? &Cache::getInstance()->firstPlayer : &Cache::getInstance()->secondPlayer;
    bool gameShouldBeEnded = false;

    if (firstPlayer->getRole() == winnerRole){
        roundWinner = &firstPlayer;
    }else{
        roundWinner = &secondPlayer;
    }
    if (Cache::getInstance()->firstRoundWinner == nullptr){
        Cache::getInstance()->firstRoundWinner = roundWinner;
    }else{
        Cache::getInstance()->secondRoundWinner = roundWinner;
    }

    QJsonObject res;
    if (Cache::getInstance()->secondRoundWinner != nullptr){
        res["state"] = "GameEnded";
        if (Cache::getInstance()->secondRoundWinner == Cache::getInstance()->firstRoundWinner){
            res["result"] = "win";
            res["winner"] = (*Cache::getInstance()->secondRoundWinner)->getUsername();
        }else{
            res["result"] = "draw";
        }
        DB::getInstance()->saveInToHistory({});
        gameShouldBeEnded = true;
    }else{
        res["state"] = "roundWinner";
        res["message"] = (*roundWinner)->getUsername() + " Won the first Round";
        res["round"] = "2";
        firstPlayer->setRole(firstPlayer->getRole() == "zombie" ? "plant" : "zombie");
        secondPlayer->setRole(secondPlayer->getRole() == "zombie" ? "plant" : "zombie");
    }

    res["role"] = firstPlayer->getRole();
    firstPlayer->socket->write(res);

    res["role"] = secondPlayer->getRole();
    secondPlayer->socket->write(res);
    if (gameShouldBeEnded){
        Cache::getInstance()->endTheGame();
    }
}
