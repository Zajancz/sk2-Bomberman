#include "ServerEventHandler.h"
using namespace Server;

ServerEventHandler::ServerEventHandler(int * serverSocketPtr, std::unordered_set<Client*> * clientsPtr) {
    serverSocket = serverSocketPtr;
    clients = clientsPtr;
}

void ServerEventHandler::handleEventEpollin(uint32_t events) {
    printf("handling new client\n");
    if(events & EPOLLIN){
        sockaddr_in clientAddr{};
        socklen_t clientAddrSize = sizeof(clientAddr);
        
        auto clientFd = accept(*serverSocket, (sockaddr*) &clientAddr, &clientAddrSize);
        if(clientFd == -1) error(1, errno, "accept failed");
        
        printf("new connection from: %s:%hu (fd: %d)\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), clientFd);
        
        new Client(clientFd);
        printf("size of clients according to serverhandler: %d\n", (int)clients->size());
        
    }
    if(events & ~EPOLLIN){
        error(0, errno, "Event %x on server socket", events);
        ctrl_c(SIGINT);
    }
}

void ServerEventHandler::handleEventEpollout(uint32_t events) {
    
    if(events & EPOLLOUT){
        printf("Test of Epollout in severEventHandler\n");
    }
}


void ServerEventHandler::ctrl_c(int){
    for(Client * client : *clients)
        delete client;
    close(*serverSocket);
    printf("Closing server\n");
    exit(0);
}