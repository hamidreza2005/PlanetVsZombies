#include "Cache.h"

Cache* Cache::instance = nullptr;
Cache::Cache() {}

Cache *Cache::getInstance() {
    if (!instance){
        instance = new Cache;
    }
    return Cache::instance;
}

bool Cache::isThereAGamePlaying() const {
    return this->firstPlayer != nullptr && this->secondPlayer != nullptr;
}

void Cache::endTheGame() {
    delete firstPlayer;
    delete secondPlayer;
    this->firstPlayer = nullptr;
    this->secondPlayer = nullptr;
}
