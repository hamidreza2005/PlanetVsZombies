#ifndef PLANETVSZOMBIES_PLAYER_H
#define PLANETVSZOMBIES_PLAYER_H
#include "QString"

class Player {
private:
    QString username;
public:
    explicit Player(QString username);

    QString getUsername();
};


#endif //PLANETVSZOMBIES_PLAYER_H
