#pragma once
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <error.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <unordered_map>
#include <iterator>
#include <list>
#include <signal.h>
#include "Messages/Text.h"
#include "../game/Player.h"
#include "../game/Bomb.h"

namespace Server {
    /* This class manages the game state from the server's perspective */
    class GameManager {
    private:
        // maps client fd onto Player
        std::unordered_map<int, Game::Player> players;
    public:
        
        GameManager();
        // virtual ~GameManager();
        void addPlayer(int fd);
        void addBomb(Bomb bomb);
        void updatePlayerPosition(int fd, PlayerPosition position);
        AllPlayersPositions getPlayersPositions();
        AllPlayersPositions getEnemiesPositions(int playerId);
        Bomb getBomb(int fd); // return last bomb player doesnt know about
    };
}