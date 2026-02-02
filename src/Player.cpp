#include "Player.h"

#include "Game.h"

Player::Player(float x, float y) {
  shape.setSize({size, size});
  shape.setPosition({x, y});

  invinsibilityTimer.stop();
}

Player::~Player() {}

void Player::update(const float delta, const sf::RenderWindow &window,
                    Game *game) {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
    shape.move({0, -speed * delta});
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
    shape.move({0, speed * delta});
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
    shape.move({-speed * delta, 0});
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
    shape.move({speed * delta, 0});
  }

  if (shape.getPosition().x < 0) {
    shape.setPosition({0, shape.getPosition().y});
  }
  if (shape.getPosition().y < 0) {
    shape.setPosition({shape.getPosition().x, 0});
  }
  if (shape.getPosition().x + size > window.getSize().x) {
    shape.setPosition({window.getSize().x - size, shape.getPosition().y});
  }
  if (shape.getPosition().y + size > window.getSize().y) {
    shape.setPosition({shape.getPosition().x, window.getSize().y - size});
  }

  if (invinsibility) {
    if (invinsibilityTimer.getElapsedTime().asSeconds() >=
        invinsibilityDuration) {
      invinsibility = false;
      invinsibilityTimer.stop();
      shape.setFillColor({255, 255, 255, 255});
    }
  }
}

void Player::render(sf::RenderWindow &window) { window.draw(shape); }

void Player::removeHP(int _hp, Game *game) {
  hp -= _hp;
  if (hp <= 0) {
    game->setGameOver();
  }
}

void Player::startInvinsibility() {
  invinsibility = true;
  invinsibilityTimer.restart();
  shape.setFillColor({255, 255, 255, 100});
}

void Player::reset() {
  shape.setPosition({375, 500});
  hp = 1;
}
