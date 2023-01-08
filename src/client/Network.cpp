#include "Network.h"
// #include "NetworkAgent.h"

using namespace Client;

//Server::Client Network::client;
int Network::test = 3;

void Network::connectToServer(char * _ip, char * _port) {
    Network::test = 12;
    // Resolve arguments to IPv4 address with a port number
    addrinfo *resolved, hints={.ai_flags=0, .ai_family=AF_INET, .ai_socktype=SOCK_STREAM};
    int res = getaddrinfo(_ip, _port, &hints, &resolved);
    if(res || !resolved) error(1, 0, "getaddrinfo: %s", gai_strerror(res));
    
    // create socket
    int sock = socket(resolved->ai_family, resolved->ai_socktype, 0);
    if(sock == -1) error(1, errno, "socket failed");
    
    // attept to connect
    res = connect(sock, resolved->ai_addr, resolved->ai_addrlen);
    if(res) error(1, errno, "connect failed");
    
    // free memory
    freeaddrinfo(resolved);

    int epollFd = epoll_create1(0);
    Server::Client::epollFd = epollFd;
    // NetworkAgent::epollFd = epollFd;
    printf("epollFd is set to: %d\n", epollFd);
    Server::Client * client = new Server::Client(sock);
    // Network::client = Server::Client(sock);
    // Custom client side solution
    //NetworkAgent::epollFd = epollFd;
    //NetworkAgent * agent = new NetworkAgent(sock);

    epoll_event epollEvent;
    epollEvent.events = EPOLLIN;

    Client::MessageHandler msgHandler(client, NULL, 0);
    //Client::MessageHandler msgHandler(client, NULL, 0);
    Text text = {"Testing client side", 0, "testing"};
    // msgHandler.sendMessage<Text>(text);

    // PlayerPosition position {88, 12};
    // msgHandler.sendMessage<Text>(text);
    // msgHandler.sendMessage<PlayerPosition>(position);

    while(1) {
        epoll_wait(epollFd, &epollEvent, 1, -1);
        printf("-- received something (potentially parial message) --\n");
        // client.handleEventEpollin(epollEvent.events);
        if (epollEvent.events & EPOLLIN)
            ((EventHandler*)epollEvent.data.ptr)->handleEventEpollin(epollEvent.events);
        else if (epollEvent.events & EPOLLOUT) {
            ((EventHandler*)epollEvent.data.ptr)->handleEventEpollout(epollEvent.events);
        }
    }
}