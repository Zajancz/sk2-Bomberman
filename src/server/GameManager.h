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
#include <list>
#include <signal.h>
#include "Messages/Text.h"
#include "Player.h"

namespace Server {
    /* This class manages the game state from the server's perspective */
    class GameManager {
    private:
        // maps client fd onto Player
        std::unordered_map<int, Player> players;
    public:
        
        GameManager();
        // virtual ~GameManager();
        void addPlayer(int fd);
        void updatePlayerPosition(int fd, PlayerPosition position);
        AllPlayersPositions getPlayersPositions();

        
    };
}