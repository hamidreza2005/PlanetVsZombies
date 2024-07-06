#ifndef SERVER_PLAYER_H
#define SERVER_PLAYER_H
#include "QString"
#include "TcpSocket.h"

class Player {
private:
    QString username;
    QString role;
public:
    explicit Player(QString username,TcpSocket* socket);

    QString getUsername();
    QString getRole();
    void setRole(QString role);
    TcpSocket* socket;
};


#endif //SERVER_PLAYER_H
