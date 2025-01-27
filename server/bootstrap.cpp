#include <QJsonDocument>
#include <QJsonObject>
#include "bootstrap.h"
#include "iostream"
#include "QThread"
#include "Controllers/Controller.h"
#include "TcpSocket.h"
#include "Cache.h"
#include "Database/DB.h"

using namespace std;

const int Bootstrap::PORT = 2000;
const QString Bootstrap::HOST = "0.0.0.0";
Bootstrap* Bootstrap::instance = nullptr;
Bootstrap::Bootstrap()
{
    this->controller = new Controller;
    this->controller->initializeRoutes();
    this->run();
}

Bootstrap::~Bootstrap()
{
    delete server;
    delete controller;
}

void Bootstrap::server_newConnection()
{
    QTcpSocket *new_client = server->nextPendingConnection();
    new_client->setObjectName("Client " + QString::number(clients.size() + 1));
    cout << "New Client Arrived" << endl;
    clients.append(new_client);
    connect(new_client, &QTcpSocket::connected, this, &Bootstrap::socket_connected);
    connect(new_client, &QIODevice::readyRead, this, [this, new_client](){ socket_readyRead(new_client); });
    connect(new_client, &QIODevice::bytesWritten, this, [this, new_client](){ socket_bytesWritten(new_client); });
    connect(new_client, &QAbstractSocket::disconnected, this, [this, new_client](){ socket_disconnected(new_client); });
}

void Bootstrap::socket_connected()
{
    cout << "Connected\n";
}

void Bootstrap::socket_readyRead(QTcpSocket *socket)
{
    QByteArray data = socket->readAll();
    QString response = QString::fromUtf8(data).trimmed();
    if(response.isEmpty()){
        return;
    }
    QVector<QString> responseParts = response.split("||").toVector();

    if(responseParts.size() < 2){
        qDebug() << "request is invalid";
        return;
    }

    QString routeName = responseParts.first();
    responseParts.pop_front();
    QJsonDocument doc = QJsonDocument::fromJson(responseParts.first().toUtf8());
    QJsonObject json = doc.object();
    auto* sock = new TcpSocket(socket);

    if (!Controller::hasRoute(routeName)){
        socket->write("Error:404 not found\n");
        return;
    }

    qDebug() << "Route " << routeName << " Called By " << socket->objectName();
    Controller::getAction(routeName)(sock,json);
}

void Bootstrap::socket_bytesWritten(QTcpSocket *_socket)
{
//    cout << "Data was written!" << endl;
}

void Bootstrap::socket_disconnected(QTcpSocket *_socket)
{
    if(Cache::getInstance()->isThereAGamePlaying()){
        qDebug() << "trying to signal other player";
        this->sendEndGameSignals(_socket);
    }
    qDebug() << (_socket->objectName() + " disconnected!");
    clients.removeOne(_socket);

    for (int var = 0; var < clients.size(); ++var)
    {
        clients[var]->setObjectName("Client " + QString::number(var + 1));
    }

    _socket->deleteLater();
}

void Bootstrap::run()
{
    server = new QTcpServer();
    server->listen(QHostAddress::Any, PORT);

    if (server->isListening())
    {
        connect(server, &QTcpServer::newConnection, this, &Bootstrap::server_newConnection);
        qDebug() << "Server is ready on " << HOST << ":" << PORT;
    }
}

Bootstrap *Bootstrap::getInstance() {
    if (!instance){
        instance = new Bootstrap;
    }
    return Bootstrap::instance;
}

QList<QTcpSocket *> Bootstrap::getClients() {
    return this->clients;
}

void Bootstrap::sendEndGameSignals(QTcpSocket* exitedPlayer) {
    QJsonObject response;
    response["state"] = "opponentLeft";
    Player* firstPlayer = Cache::getInstance()->firstPlayer;
    Player* secondPlayer = Cache::getInstance()->secondPlayer;
    if (exitedPlayer == firstPlayer->socket->getOriginalSocket()){
        if(secondPlayer->socket->isConnected())
           secondPlayer->socket->write(response);
        this->saveGameInHistory(secondPlayer,firstPlayer);
        Cache::getInstance()->endTheGame();
    }else if(exitedPlayer == secondPlayer->socket->getOriginalSocket()){
        if(firstPlayer->socket->isConnected())
            firstPlayer->socket->write(response);
        this->saveGameInHistory(firstPlayer,secondPlayer);
        Cache::getInstance()->endTheGame();
    }
}

void Bootstrap::saveGameInHistory(Player *leftPlayer,Player* existedPlayer) {
    DB::getInstance()->saveInToHistory({
       {"firstPlayer",leftPlayer->getUsername()},
       {"secondPlayer",existedPlayer->getUsername()},
       {"firstRoundWinner",leftPlayer->getUsername()},
       {"secondRoundWinner",leftPlayer->getUsername()},
       {"result","win"},
   });
}
