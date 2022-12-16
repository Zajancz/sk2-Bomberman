#include "ServerAction.h"

std::unordered_set<Client*> * ServerAction::clients;
int ServerAction::serverSocket;
int ServerAction::epollFd;

/// @brief Starts up a server
/// @param port - port number
/// @param clients - pointer to client set
/// @return server socket
int ServerAction::start(int port, std::unordered_set<Client*> * clientsPtr) {
    printf("starting the server\n");
    int servFd = socket(AF_INET, SOCK_STREAM, 0);
    if(servFd == -1) error(1, errno, "socket failed");
    // Setting static variables of ServerAction class
    serverSocket = servFd;
    clients = clientsPtr;
    epollFd = epoll_create1(0);
    Client::epollFd = epollFd;
    printf("epollFd is set to: %d\n", epollFd);
    
    signal(SIGINT, ctrl_c);
    signal(SIGPIPE, SIG_IGN);
    
    setReuseAddr();
    
    sockaddr_in serverAddr{.sin_family=AF_INET, .sin_port=htons((short)port), .sin_addr={INADDR_ANY}};
    int res = bind(servFd, (sockaddr*) &serverAddr, sizeof(serverAddr));
    if(res) error(1, errno, "bind failed");
    
    res = listen(servFd, 1);
    if(res) error(1, errno, "listen failed");
    printf("server started\n");
    return servFd;
}

void ServerAction::ctrl_c(int){
    for(Client * client : *clients)
        delete client;
    close(serverSocket);
    printf("Closing server\n");
    exit(0);
}

void ServerAction::setReuseAddr(){
    const int one = 1;
    int res = setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    if(res) error(1,errno, "setsockopt failed");
}