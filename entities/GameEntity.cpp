#include "GameEntity.h"

GameEntity::GameEntity(int health):health(health) {

}

int GameEntity::getHealth() const {
    return health;
}

void GameEntity::setHealth(int newHealth) {
    health = newHealth;
}

void GameEntity::setImage() {
    QPixmap image_ground(getPicturePath());
    QPixmap Scaled_image_ground = image_ground.scaled(75,75,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    setPixmap(Scaled_image_ground);
}
