#include "GameManager.h"
#include <time.h>

using namespace Server;

/**
 * Overall this file is used by Godot and C++ to make available communication between them
 * trough a list of functions listed below 
*/

GameManager::GameManager() {

}

/**
 * @brief This function is used to add player to the map in Godot engine
 * 
 * @param fd stores a file descriptor of a current client
*/

void GameManager::addPlayer(int fd) {
    PlayerPosition position = {x: 50, y: 50};
    std::pair<int, Game::Player> pair (fd, Game::Player {fd, position});
    players.insert(pair);
}

/**
 * @brief This function is used to add bombs from the client to the map in Godot engine
 * 
 * After assigning values to a variable we need to show this boms to all players(enemies)
 * on the map
 * 
 * @param _bomb stores a file descriptor of a current client and bomb position
*/

void GameManager::addBomb(Bomb _bomb) {
    printf("Server::GameManager::addBomb Placing bomb\n");
    Game::Bomb bomb {_bomb.fd, _bomb.position};
    auto p = players.begin();
    while (p != players.end()) {
        if (p->first != _bomb.fd) {
            p->second.bombs.push_back(bomb);
            printf("\tBomb placed by %d added to player %d\n", _bomb.fd, p->first);
        }
        p++;
    }
}

/**
 * @brief This function is used to update player position
 *
 * To be able to move on the map so every enemy could see your moves
 * we need this function. We search for a client moving by fd, if didnt
 * find it, then we return, else we store a new position of a client 
 * 
 * @param fd stores a file descriptor of a current client
 * @param position stores a position of a current client
*/

void GameManager::updatePlayerPosition(int fd, PlayerPosition position) {
    auto p = players.find(fd);
    if (p == players.end()) {
        printf("Warning: Unexpected key in GameManager.players.map");
        return;
    }
    p->second.position = position;
}

/**
 * @brief This function is used to get current position of a player
 *
 * Mostly created for Godot engine needs
 * 
 * @return a current player position
*/

AllPlayersPositions GameManager::getPlayersPositions() {
    AllPlayersPositions pos;
    memset(&pos, 0, sizeof(pos)); // setting zeroes
    int i = 0;
    for(auto kv : players) {
        pos.ids[i] = kv.first;
        pos.position[i] = kv.second.position;
        i++;
    } 
    return pos;
}

/**
 * @brief This function is used to find all enemies position
 *
 * @param playerId stores a file descriptor of a current client
 * 
 * @return all enemies of a given player
*/

AllPlayersPositions GameManager::getEnemiesPositions(int playerId) {
    AllPlayersPositions pos;
    memset(&pos, 0, sizeof(pos)); // setting zeroes
    int i = 0;
    for(auto kv : players) {
        if (kv.first != playerId) {
            pos.ids[i] = kv.first;
            pos.position[i] = kv.second.position;
            i++;
        }
    } 
    return pos;
}

/**
 * @brief This function is used to set a bomb by client on his position
 *
 * @param fd stores a file descriptor of a current client
 * 
 * @return a file descriptor of the player placed a bomb and bomb position
*/

Bomb GameManager::getBomb(int fd) {
    printf("Getting a bomb for user %d\n", fd);
    auto p = players.find(fd);
    if (p == players.end()) { printf("GameManager::getBomb - Warning: Unexpected map key"); return Bomb {}; }
    printf("There are %d bombs client needs to know about", (int)p->second.bombs.size());
    if (p->second.bombs.size() == 0) return Bomb {0};
    Game::Bomb gameBomb = *p->second.bombs.begin();
    p->second.bombs.pop_front();
    return Bomb {gameBomb.fd, gameBomb.position};
}