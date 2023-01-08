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

namespace Server {
    /* This class a single player in the game */
    class Player {
    private:
        
    public:
        int fd;
        PlayerPosition position;
        Player(int _fd, PlayerPosition _pos);
    };
}