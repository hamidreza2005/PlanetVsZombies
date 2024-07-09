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
    void setUsername(QString newUsername);
    QString& getRole();
};


#endif //PLANETVSZOMBIES_PLAYER_H
