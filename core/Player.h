#ifndef PLANETVSZOMBIES_PLAYER_H
#define PLANETVSZOMBIES_PLAYER_H
#include "QString"

class Player {
private:
    QString username;
    QString role;
public:
    explicit Player(QString username);

    QString getUsername();
    QString& getRole();
};


#endif //PLANETVSZOMBIES_PLAYER_H
