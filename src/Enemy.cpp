#include "Enemy.h"

#include "Player.h"
#include "ResourceManager.h"

Enemy::Enemy(float x, float y, float _speed) {
  shape.setSize({size, size});
  shape.setPosition({x, y});
  shape.setFillColor(sf::Color::Red);
  speed = _speed;
}

Enemy::~Enemy() {}

void Enemy::update(const float delta, Player &player, Game *game) {
  shape.move({0, speed * delta});

  if (checkCollision(player)) {
    player.removeHP(1, game);
    shape.setPosition({0, 1000});
    ResourceManager::hitSound.play();
  }
}

void Enemy::render(sf::RenderWindow &window) { window.draw(shape); }

bool Enemy::checkCollision(Player &player) const {
  if (!player.isInvinsible()) {
    if (shape.getGlobalBounds().findIntersection(player.getGlobalBounds())) {
      return true;
    }
  }
  return false;
}
