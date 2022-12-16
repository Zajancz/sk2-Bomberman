#pragma once

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unordered_set>
#include <sys/epoll.h>
#include "Client.h"

// class Client;

/// @brief ServerAction class gathers methods for managing the server
class ServerAction {
    static std::unordered_set<Client*> * clients;
    static int serverSocket;
    static void setReuseAddr();
public:
    static int epollFd;
    static int start(int port, std::unordered_set<Client*> * clients);
    static void ctrl_c(int);
    static int getEpollFd() { return epollFd; };
};