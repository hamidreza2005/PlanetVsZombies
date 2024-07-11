#include <QNetworkInterface>
#include <QJsonArray>
#include "GameController.h"
#include "../bootstrap.h"
#include "../Cache.h"
#include "../Database/DB.h"

QString getServerIp(){
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol &&
            address != QHostAddress(QHostAddress::LocalHost)) {
            return address.toString();
        }
    }
    return "";
}
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
            if(ipAddress == "127.0.0.1"){
                auto deviceIp = getServerIp();
                ipAddress = deviceIp != "" ? deviceIp : ipAddress;
            }
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
    response["opponent"] = Cache::getInstance()->secondPlayer->getUsername();
    Cache::getInstance()->firstPlayer->setRole(firstPlayerRole);
    Cache::getInstance()->firstPlayer->socket->write(response);

    response["role"] = secondPlayerRole;
    response["opponent"] = Cache::getInstance()->firstPlayer->getUsername();
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
    bool requestIsSentByFirstPlayer = socket->getOriginalSocket() == firstPlayer->socket->getOriginalSocket();
    bool requestIsSentBySecondPlayer = socket->getOriginalSocket() == secondPlayer->socket->getOriginalSocket();

    if (!requestIsSentByFirstPlayer && !requestIsSentBySecondPlayer){
        socket->sendValidationError("game","you can't interfere in the game");
        return;
    }
    if (request["state"] == "playerResigned"){
        QJsonObject res;
        res["state"] = "GameEnded";
        res["result"] = "win";
        Player* winner;
        if (requestIsSentByFirstPlayer){
            winner = secondPlayer;
            res["winner"] = secondPlayer->getUsername();
        }else{
            winner = firstPlayer;
            res["winner"] = firstPlayer->getUsername();
        }
        DB::getInstance()->saveInToHistory({
           {"firstPlayer",firstPlayer->getUsername()},
           {"secondPlayer",secondPlayer->getUsername()},
           {"firstRoundWinner",winner->getUsername()},
           {"secondRoundWinner",winner->getUsername()},
           {"result","win"},
        });

        firstPlayer->socket->write(res);
        secondPlayer->socket->write(res);
        return;
    }

    if (request["state"] == "zombieReachedTheEnd"){
        if ((requestIsSentByFirstPlayer && firstPlayer->getRole() == "zombie") || (requestIsSentBySecondPlayer && secondPlayer->getRole() == "zombie")){
            GameController::handleEndOfTheGame("zombie");
        }
        return;
    }


    if(request["state"] == "timeEnded" && requestIsSentByFirstPlayer){
        GameController::handleEndOfTheGame("plant");
        return;
    }

    if (request["state"] == "add" || request["state"] == "chat"){
        QJsonObject response = request;
        if(requestIsSentByFirstPlayer){
            secondPlayer->socket->write(response);
        }else{
            firstPlayer->socket->write(response);
        }
    }
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
        DB::getInstance()->saveInToHistory({
            {"firstPlayer",firstPlayer->getUsername()},
            {"secondPlayer",secondPlayer->getUsername()},
            {"firstRoundWinner",(*Cache::getInstance()->firstRoundWinner)->getUsername()},
            {"secondRoundWinner",(*Cache::getInstance()->secondRoundWinner)->getUsername()},
            {"result",res["result"].toString()},
        });
        gameShouldBeEnded = true;
    }else{
        res["state"] = "roundWinner";
        res["message"] = (*roundWinner)->getUsername() + " Won the first Round";
        res["round"] = "2";
        GameController::switchPlayersRoles(firstPlayer,secondPlayer);
    }

    res["role"] = firstPlayer->getRole();
    firstPlayer->socket->write(res);

    res["role"] = secondPlayer->getRole();
    secondPlayer->socket->write(res);
    if (gameShouldBeEnded){
        Cache::getInstance()->endTheGame();
    }
}

void GameController::switchPlayersRoles(Player *firstPlayer, Player *secondPlayer) {
    auto temp = firstPlayer->getRole();
    firstPlayer->setRole(secondPlayer->getRole());
    secondPlayer->setRole(temp);
}

void GameController::getHistory(TcpSocket *socket, const QJsonObject &request) {
    auto username = request.value("username").toString();
    auto history = DB::getInstance()->findHistory(username);
    QJsonObject response;
    response["status"] = "200";
    response["history"] = history;
    socket->write(response);
}

void GameController::declineInvite(TcpSocket* socket,const QJsonObject& request){
    if (!Cache::getInstance()->firstPlayer){
        socket->sendValidationError("invitation","there is no invitation to decline",404);
        return;
    }
    QJsonObject response;
    response["username"] = request.value("username");
    response["state"] = "invitationDeclined";
    Cache::getInstance()->firstPlayer->socket->write(response);
}