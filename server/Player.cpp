#include "Player.h"

Player::Player(QString username,TcpSocket* socket):username(username),socket(socket) {

}

QString Player::getUsername() {
    return this->username;
}

QString Player::getRole() {
    return this->role;
}

void Player::setRole(QString newRole) {
    this->role = newRole;
}
