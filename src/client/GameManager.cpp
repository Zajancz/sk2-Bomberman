#include "GameManager.h"

using namespace Client;

std::unordered_map<int, Game::Player> GameManager::enemies;
std::list<Game::Bomb> GameManager::bombs;

void GameManager::addEnemy(int id, PlayerPosition position) {
    // for adding new
    std::pair<int, Game::Player> pair (id, Game::Player { id, position } );
    GameManager::enemies.insert(pair);
    // for updating - i know it's inefficient to do both here, whatever
    GameManager::enemies[id] = Game::Player { id, position };
}
void GameManager::addBomb(Bomb _bomb) {
    printf("Client::GameManager::addBomb - Received bomb\n");
    Game::Bomb bomb {_bomb.fd, _bomb.position};
    bombs.push_back(bomb);
}
Bomb GameManager::getBomb() {
    printf("Getting bomb info\n");
    printf("There are %d bombs player needs to know about", (int)GameManager::bombs.size());
    if (GameManager::bombs.size() == 0) return Bomb {0};
    Game::Bomb gameBomb = *GameManager::bombs.begin();
    GameManager::bombs.pop_front();
    return Bomb {gameBomb.fd, gameBomb.position};
}