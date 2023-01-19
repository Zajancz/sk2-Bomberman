#include "GameManager.h"
#include <time.h>

using namespace Server;

GameManager::GameManager() {

}
void GameManager::addPlayer(int fd) {
    PlayerPosition position = {x: 50, y: 50};
    std::pair<int, Game::Player> pair (fd, Game::Player {fd, position});
    players.insert(pair);
}
void GameManager::addBomb(Bomb _bomb) {
    printf("Server::GameManager::addBomb Placing bomb\n");
    Game::Bomb bomb {_bomb.fd, _bomb.position};
    // adding bomb to all players that need to know about it (all enemies)
    auto p = players.begin();
    while (p != players.end()) {
        if (p->first != _bomb.fd) {
            p->second.bombs.push_back(bomb);
            printf("\tBomb placed by %d added to player %d\n", _bomb.fd, p->first);
        }
        p++;
    }
}
void GameManager::updatePlayerPosition(int fd, PlayerPosition position) {
    auto p = players.find(fd);
    if (p == players.end()) {
        printf("Warning: Unexpected key in GameManager.players.map");
        return;
    }
    p->second.position = position;
}
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
/// @return all enemies of a given player
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