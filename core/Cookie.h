#ifndef PLANETVSZOMBIES_COOKIE_H
#define PLANETVSZOMBIES_COOKIE_H
#include "Player.h"

class Cookie {
private:
    static Cookie* instance;
public:
    Cookie() = default;
    static Cookie* getInstance();
    Player* loggedInPlayer = nullptr;
    QString opponentUsername = "";
    QString playingRound = "0";
    int zombiesCountThatReachedTheEnd = 0;
};


#endif //PLANETVSZOMBIES_COOKIE_H
