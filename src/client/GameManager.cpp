#include "GameManager.h"

using namespace Client;

std::unordered_map<int, Game::Player> GameManager::enemies;
std::list<Game::Bomb> GameManager::bombs;

/**
 * @brief This function mainly created for Godot engine use
 * 
 * With it we can add an enemy to the map
 * 
 * @param id - id of an enemy
 * @param position - position of an enemy
 * 
*/

void GameManager::addEnemy(int id, PlayerPosition position) {
    // for adding new
    std::pair<int, Game::Player> pair (id, Game::Player { id, position } );
    GameManager::enemies.insert(pair);
    // for updating - i know it's inefficient to do both here, whatever
    GameManager::enemies[id] = Game::Player { id, position };
}

/**
 * @brief This function mainly created for Godot engine use
 * 
 * With it we can add a bomb to the map
 * 
 * @param _bomb - have a file descriptor of player placed a bomb and a position
 * 
*/

void GameManager::addBomb(Bomb _bomb) {
    printf("Client::GameManager::addBomb - Received bomb\n");
    Game::Bomb bomb {_bomb.fd, _bomb.position};
    bombs.push_back(bomb);
}

/**
 * @brief This function mainly created for Godot engine use
 * 
 * With it we can check all bombs on the map
 * 
 * @return - a file descriptor of player placed a bomb and a position with gameBomb class
 * 
*/

Bomb GameManager::getBomb() {
    printf("Getting bomb info\n");
    printf("There are %d bombs player needs to know about", (int)GameManager::bombs.size());
    if (GameManager::bombs.size() == 0) return Bomb {0};
    Game::Bomb gameBomb = *GameManager::bombs.begin();
    GameManager::bombs.pop_front();
    return Bomb {gameBomb.fd, gameBomb.position};
}