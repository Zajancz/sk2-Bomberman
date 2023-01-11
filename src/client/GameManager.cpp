#include "GameManager.h"

using namespace Client;

std::unordered_map<int, Game::Player> GameManager::enemies;
// std::unordered_set<Game::Bomb> GameManager::bombs;

void GameManager::addEnemy(int id, PlayerPosition position) {
    std::pair<int, Game::Player> pair (id, Game::Player { id, position } );
    GameManager::enemies.insert(pair);
}