#include "GameEntity.h"

GameEntity::GameEntity(int health):health(health) {

}

int GameEntity::getHealth() const {
    return health;
}

void GameEntity::setHealth(int newHealth) {
    health = newHealth;
}