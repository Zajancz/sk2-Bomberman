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
#include <unordered_set>
#include <list>
#include <signal.h>
#include "EventHandler.h"
#include "Buffer.h"
#include "GameManager.h"

namespace Server {
    class Client : public EventHandler {
    private:
        int _fd;
        Buffer readBuffer;
        std::list<Buffer> dataToWrite;

        void waitForWrite(bool epollout);
        // void handleMessage(int length);
    public:
        static int epollFd;
        static std::unordered_set<Client*> clients;

        Client(int fd);
        virtual ~Client();

        int fd() const;
        GameManager* gameManager; // The game, the client is playing

        virtual void handleEventEpollin(uint32_t events) override;
        virtual void handleEventEpollout(uint32_t events) override;
        void write(char * buffer, int count);
        void remove();
        void sendToAllBut(int fd, char * buffer, int count);
    };
}