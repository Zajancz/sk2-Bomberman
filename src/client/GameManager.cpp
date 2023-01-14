#include "GameManager.h"

using namespace Client;

std::unordered_map<int, Game::Player> GameManager::enemies;
// std::unordered_set<Game::Bomb> GameManager::bombs;

void GameManager::addEnemy(int id, PlayerPosition position) {
    // for adding new
    std::pair<int, Game::Player> pair (id, Game::Player { id, position } );
    GameManager::enemies.insert(pair);
    // for updating - i know it's inefficient to do both here, whatever
    GameManager::enemies[id] = Game::Player { id, position };
}