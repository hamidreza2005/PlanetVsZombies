#include "GameEntity.h"

GameEntity::GameEntity(int health,int cost): health(health), cost(cost) {

}

int GameEntity::getHealth() const {
    return health;
}

void GameEntity::setHealth(int newHealth) {
    health = newHealth;
}

void GameEntity::setImage() {
    QPixmap image_ground(getPicturePath());
    QPixmap Scaled_image_ground = image_ground.scaled(75,75,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    setPixmap(Scaled_image_ground);
}

void GameEntity::setCost(int newCost) {
    this->cost = newCost;
}

int GameEntity::getCost() const {
    return this->cost;
}
