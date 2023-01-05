#pragma once

#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <error.h>
#include <netdb.h>
#include <cstdio>
#include <cstring>
#include <sys/epoll.h>
#include "../server/Messages/Text.h"
#include "../server/Client.h"
#include "../server/MessageHandler.h"
#include "Network.h"

namespace Client {
    /// @brief Network class establishes connection to the server
    class Network {
    public:
        static void connectToServer(char* _ip, char* _port);
    };
}