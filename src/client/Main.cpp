//      Contents of this file will in future be moved to Network class in a separate file.
//      Starting point for this file is 05_sockets_concurrency/z1_funkcja.cpp

#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <error.h>
#include <netdb.h>
#include <cstdio>
#include <cstring>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include "../server/Messages/Text.h"
#include "../server/Client.h"
#include "../server/MessageHandler.h"

ssize_t readData(int fd, char * buffer, ssize_t buffsize){
    auto ret = read(fd, buffer, buffsize);
    if(ret==-1) error(1,errno, "read failed on descriptor %d", fd);
    return ret;
}

void writeData(int fd, char * buffer, ssize_t count){
    auto ret = write(fd, buffer, count);
    if(ret==-1) error(1, errno, "write failed on descriptor %d", fd);
    if(ret!=count) error(0, errno, "wrote less than requested to descriptor %d (%ld/%ld)", fd, count, ret);
}

void socketReaderFunction(int sock){
        while(1){
            // read from socket, write to stdout
            ssize_t bufsize = 255, received;
            char buffer[bufsize];
            received = readData(sock, buffer, bufsize);
            if(received <= 0){
                shutdown(sock, SHUT_RDWR);
                close(sock);
                exit(0);
            }
            writeData(1, buffer, received);
            
        }
}

int main(int argc, char ** argv){

    if(argc!=3) error(1,0,"Need 2 args");
    
    // Resolve arguments to IPv4 address with a port number
    addrinfo *resolved, hints={.ai_flags=0, .ai_family=AF_INET, .ai_socktype=SOCK_STREAM};
    int res = getaddrinfo(argv[1], argv[2], &hints, &resolved);
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
    printf("epollFd is set to: %d\n", epollFd);
    Server::Client * client = new Server::Client(sock);
    epoll_event epollEvent;
    epollEvent.events = EPOLLIN;

    Server::MessageHandler msgHandler(client, NULL, 0);
    Text text = {"Testing client side", 0, "testing"};
    msgHandler.sendMessage<Text>(text);
    //client->write()

    while(1) {
        printf("\tepoll_wait: %d \n",(int)epoll_wait(epollFd, &epollEvent, 1, -1));
        printf("========= about to handle request of known client ===========\n");
        // client.handleEventEpollin(epollEvent.events);
        if (epollEvent.events & EPOLLIN)
            ((EventHandler*)epollEvent.data.ptr)->handleEventEpollin(epollEvent.events);
        else if (epollEvent.events & EPOLLOUT) {
            ((EventHandler*)epollEvent.data.ptr)->handleEventEpollout(epollEvent.events);
        }
    }
    return 0;
}