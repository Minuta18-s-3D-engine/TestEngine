#include "Player.hpp"

Player::Player(glm::vec3 pos) : pos(pos) {
    this->camera = std::make_shared<Camera>(
        Camera(pos, glm::vec3(0.0f, 1.0f, 0.0f)));
}

Player::~Player() = default;

std::shared_ptr<Camera> Player::getCamera() {
    return camera;
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
    camera->pos = pos;
    // std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
}

glm::vec3 Player::getPos() {
    return pos;
}
