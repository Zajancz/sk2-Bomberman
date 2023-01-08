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
// #include "../server/Client.h"
#include "../server/EventHandler.h"
#include "../server/Buffer.h"

namespace Client {
    class NetworkAgent : public EventHandler {
        int _fd;
        Buffer readBuffer;
        std::list<Buffer> dataToWrite;

        void handleMessage(int length);
        void waitForWrite(bool epollout);
    public:
        static int epollFd;
        // ? For some reason, functions defined here, when moved to .cpp
        // ? cause a loading error of a library in godot?
        NetworkAgent(int fd){
            _fd = fd;
            printf("Creating a new NetworkAgent\n");
            printf("epollFd is set to: %d\n", epollFd);
            epoll_event ee {EPOLLIN|EPOLLRDHUP, {.ptr=this}};
            epoll_ctl(epollFd, EPOLL_CTL_ADD, _fd, &ee);
        }

        void write(char * buffer, int count);
        void remove();
        void sendToAllBut(int fd, char * buffer, int count);

        virtual void handleEventEpollin(uint32_t events) override
        {
            ssize_t count = read(_fd, readBuffer.dataPos(), readBuffer.remaining());
            if(count <= 0)
                events |= EPOLLERR;
            else {
                readBuffer.pos += count;
                // Find '\n' symbol and return pointer to it's location (nullptr if not found)
                char * eol = (char*) memchr(readBuffer.data, '\n', readBuffer.pos);
                if(eol == nullptr) { // readBuffer does not (yet) contain `\n` symbol
                    if(0 == readBuffer.remaining())
                        readBuffer.doube();
                } else { // readbuffer contains at least one `\n`
                    do {
                        auto thismsglen = eol - readBuffer.data + 1;
                        // ! err: handleMessage(thismsglen);

                        // calculate lenth of remaining data in buffer and updating buffer position
                        auto nextmsgslen =  readBuffer.pos - thismsglen;
                        memmove(readBuffer.data, eol+1, nextmsgslen);
                        readBuffer.pos = nextmsgslen;
                    } while((eol = (char*) memchr(readBuffer.data, '\n', readBuffer.pos)));
                }
            }
            if(events & ~EPOLLIN) {
                // TODO: Handle: Lost connection to the server
            }
        }
        virtual void handleEventEpollout(uint32_t events) override
        {
            do {
                int remaining = dataToWrite.front().remaining();
                int sent = send(_fd, dataToWrite.front().data+dataToWrite.front().pos, remaining, MSG_DONTWAIT);
                if(sent == remaining) {
                    dataToWrite.pop_front();
                    if(0 == dataToWrite.size()) {
                        waitForWrite(false);
                        break;
                    }
                    continue;
                } else if(sent == -1) {
                    if(errno != EWOULDBLOCK && errno != EAGAIN)
                        events |= EPOLLERR;
                } else
                    dataToWrite.front().pos += sent;
            } while(false);

            if(events & ~EPOLLOUT) {
                remove();
            }
        }
    };
};


