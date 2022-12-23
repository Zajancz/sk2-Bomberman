//      Contents of this file will in future be moved to Network class in a separate file.
//      Starting point for this file is 05_sockets_concurrency/z2cpp

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
#include <unordered_set>
#include <list>
#include <signal.h>
#include "Client.h"
#include "Buffer.h"
#include "EventHandler.h"
#include "ServerEventHandler.h"
#include "ServerAction.h"

class Client;

int servFd;
std::unordered_set<Client*> clients;

void ctrl_c(int);

uint16_t readPort(char * txt);

int main(int argc, char ** argv){
    if(argc != 2) error(1, 0, "Need 1 arg (port)");
    auto port = readPort(argv[1]);
    
    servFd = ServerAction::start(port, &clients); // that's probably the wrong clients
    // Create an epoll file descriptor
    int epollFd = ServerAction::epollFd;

    printf("server started - confirm\n");

    // Set up the event structure
    ServerEventHandler* serverEventHandler = new ServerEventHandler(&servFd, &Client::clients);
    epoll_event epollEvent;
    epollEvent.events = EPOLLIN;
    epollEvent.data.ptr = serverEventHandler;
    // {EPOLLIN, {.ptr=&serverEventHandler}};
    epollEvent.data.u64 = 987654321;
    // Add server socket (servFd) to the epoll set
    epoll_ctl(epollFd, EPOLL_CTL_ADD, servFd, &epollEvent);

    while(true) {
        // Wait for an event on the epollFd
        printf("\tepoll_wait: %d \n",(int)epoll_wait(epollFd, &epollEvent, 1, -1));
        if ( epoll_wait(epollFd, &epollEvent, 1, -1) == -1 && errno!=EINTR) {
            error(0,errno,"epoll_wait failed");
            ctrl_c(SIGINT);
        }
        if (epollEvent.data.u64 == 987654321) {
            printf("========= about to handle new request ===========\n");
            // ! this throws "Segmentation fault (core dumped)" ! // I dont understand why.
            // !((ServerEventHandler*)epollEvent.data.ptr)->handleEvent(epollEvent.events);
            // the following seems to work,
            serverEventHandler->handleEvent(epollEvent.events);
            printf("finished handling new request\n");
        } else {
            printf("========= about to handle request of known client ===========\n");
            // Client->handleEvent()
            ((EventHandler*)epollEvent.data.ptr)->handleEvent(epollEvent.events);
        }
    }

}

uint16_t readPort(char * txt){
    char * ptr;
    auto port = strtol(txt, &ptr, 10);
    if(*ptr!=0 || port<1 || (port>((1<<16)-1))) error(1,0,"illegal argument %s", txt);
    return port;
}


void ctrl_c(int){
    for(Client * client : clients)
        delete client;
    close(servFd);
    printf("Closing server\n");
    exit(0);
}