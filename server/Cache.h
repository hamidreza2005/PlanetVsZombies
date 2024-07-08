#ifndef SERVER_CACHE_H
#define SERVER_CACHE_H
#include "Player.h"

class Cache {
    static Cache* instance;
public:
    Player* firstPlayer = nullptr;
    Player* secondPlayer = nullptr;
    Player** secondRoundWinner = nullptr;
    Player** firstRoundWinner = nullptr;

    Cache();
    Cache(Cache const&) = delete;
    void operator=(Cache const&) = delete;

    static Cache* getInstance();
    bool isThereAGamePlaying() const;
    void endTheGame();
};


#endif //SERVER_CACHE_H
