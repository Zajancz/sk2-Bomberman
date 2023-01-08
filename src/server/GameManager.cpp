#include "GameManager.h"

using namespace Server;

GameManager::GameManager() {

}
void GameManager::addPlayer(int fd) {
    PlayerPosition position = {x: 50, y: 50};
    std::pair<int, Player> pair (fd, Player(fd, position));
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
    int i = 0;
    for(auto kv : players) {
        pos.ids[i] = kv.first;
        pos.position[i] = kv.second.position;
        i++;
    } 
    return pos;
}