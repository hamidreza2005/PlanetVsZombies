#include "Player.h"

Player::Player(QString username):username(username) {

}

QString Player::getUsername() {
    return this->username;
}

QString &Player::getRole() {
    return this->role;
}

void Player::setUsername(QString newUsername) {
    this->username = newUsername;
}
