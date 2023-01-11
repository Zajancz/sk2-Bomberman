#include "GameManager.h"

using namespace Server;

GameManager::GameManager() {

}
void GameManager::addPlayer(int fd) {
    PlayerPosition position = {x: 50, y: 50};
    std::pair<int, Game::Player> pair (fd, Game::Player {fd, position});
    players.insert(pair);
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