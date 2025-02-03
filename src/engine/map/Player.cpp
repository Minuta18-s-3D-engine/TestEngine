#include "Player.hpp"

Player::Player() {
    camera = Camera();
    pos = glm::vec3(0.0f);
}

Player::~Player() = default;

std::unique_ptr<Camera> Player::getCamera() {
    return std::make_unique<Camera>(camera);
}

void Player::addPos(const glm::vec3& pos) {
    this->pos += pos;
} 

void Player::setPos(const glm::vec3& pos) {
    this->pos = pos;
}

float Player::getSpeed() {
    return PLAYER_SPEED;
}

void Player::update(float deltaTime) {
    camera.pos = pos;

    std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
}
