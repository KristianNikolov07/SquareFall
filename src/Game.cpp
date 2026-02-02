#include "Game.h"

#include "Enemy.h"
#include "Heart.h"

Game::Game() : player(375, 500) {
  // Window
  window.create(sf::VideoMode({800, 800}), "Square Fall",
                sf::Style::Titlebar | sf::Style::Close);
  window.setFramerateLimit(60);

  // Seed
  srand(time(0));
}

Game::~Game() {}

void Game::HandleEvents() {
  while (const auto event = window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      window.close();
    }
  }
}

void Game::Update() {
  const float delta = clock.restart().asSeconds();

  if (gameOver == false) {

    // Player
    player.update(delta, window, this);

    // Spawning stuff
    summoner();

    // Update hearts
    for (Heart &heart : hearts) {
      heart.update(delta, player);
    }
    clearHearts();

    // Update enemies
    for (Enemy &enemy : enemies) {
      enemy.update(delta, player, this);
    }
    clearEnemies();

    // Update invinsibility powerups
    for (InvinsibilityPowerup &invinsibilityPowerup : invinsibilityPowerups) {
      invinsibilityPowerup.update(delta, player);
    }
    clearInvinsibilityPowerups();

    hpText.update(player);
    timer.update(delta);
  } else {
    gameOverScreen.update(delta, this);
  }
}

void Game::Render() {
  window.clear(sf::Color::Black);

  player.render(window);

  for (Heart &heart : hearts) {
    heart.render(window);
  }

  for (Enemy &enemy : enemies) {
    enemy.render(window);
  }

  for (InvinsibilityPowerup &invinsibilityPowerup : invinsibilityPowerups) {
    invinsibilityPowerup.render(window);
  }

  window.draw(hpText);
  timer.render(window);

  if (gameOver == true) {
    gameOverScreen.render(window);
  }

  window.display();
}

void Game::reset() {
  enemySpeed = defaultEnemySpeed;
  enemyAmount = defaultEnemyAmount;
  spawnDelay = defaultSpawnDelay;
  heartSpawnChance = defaultHeartSpawnChance;

  player.reset();
  timer.reset();

  enemies.clear();
  hearts.clear();
  invinsibilityPowerups.clear();

  clock.restart();
  gameOver = false;
}

void Game::summoner() {

  // Phase 1
  if (timer.getSeconds() > 30) {
    enemySpeed = 300;
    enemyAmount = 2;
  }

  // Phase 2
  if (timer.getSeconds() > 60) {
    enemySpeed = 500;
    enemyAmount = 3;
  }

  // Phase 3
  if (timer.getSeconds() > 90) {
    spawnDelay = 0.5;
    enemySpeed = 400;
    enemyAmount = 1;
  }

  // Phase 4
  if (timer.getSeconds() > 120) {
    enemyAmount = 3;
    enemySpeed = 500;
    spawnDelay = 0.4;
  }

  if (spawnClock.getElapsedTime().asSeconds() > spawnDelay) {
    spawnClock.restart();

    // Hearts
    int randNum = rand() % 100;
    if (randNum <= heartSpawnChance) {
      addHeart();
    }

    // Invinsibility Powerups
    randNum = rand() % 100;
    if (randNum <= invinsibilityPowerupSpawnChange) {
      addInvinsibilityPowerup();
    }

    addEnemy(enemySpeed, enemyAmount);
  }
}

void Game::addEnemy(float speed, int amount) {
  int previousPos = -1;
  for (int i = 0; i < amount; i++) {
    int randNum = rand() % 10;
    int x = randNum * 80;
    while (previousPos == x) {
      randNum = rand() % 10;
      x = randNum * 80;
    }
    previousPos = x;

    Enemy enemy(x, -120, speed);
    enemies.push_back(enemy);
  }
}

void Game::clearEnemies() {
  int i = 0;
  for (Enemy &enemy : enemies) {
    if (enemy.getPosition().y > 850) {
      enemies.erase(enemies.begin() + i);
    }
    i++;
  }
}

void Game::addHeart() {
  const int randNum = rand() % 10;
  const int x = randNum * 80;

  Heart heart(x, -120);
  hearts.push_back(heart);
}

void Game::clearHearts() {
  int i = 0;
  for (Heart heart : hearts) {
    if (heart.getPosition().y > 850) {
      hearts.erase(hearts.begin() + i);
    }
    i++;
  }
}

void Game::addInvinsibilityPowerup() {
  const int randNum = rand() % 10;
  const int x = randNum * 80;

  InvinsibilityPowerup invinsibilityPowerup(x, -120);
  invinsibilityPowerups.push_back(invinsibilityPowerup);
}

void Game::clearInvinsibilityPowerups() {
  int i = 0;
  for (InvinsibilityPowerup &invinsibilityPowerup : invinsibilityPowerups) {
    if (invinsibilityPowerup.getPosition().y > 850) {
      invinsibilityPowerups.erase(invinsibilityPowerups.begin() + i);
    }
    i++;
  }
}
