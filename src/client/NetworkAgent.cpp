#include "NetworkAgent.h"
#include "../server/EventHandler.h"
#include "../server/Buffer.h"
#include "MessageHandler.h"

using namespace Client;

int NetworkAgent::epollFd;


/**
 * @brief  This function is used to read a message from the server of specific length
 * 
 * @param length - amount of bytes we want to read from the server
 * 
*/

void NetworkAgent::handleMessage(int length) {
    printf("Handling server's message\n");
    printf("message of length: %d,\nbuffer: %s.\n", length, readBuffer.data);
    char * buffer = readBuffer.dataPos();
    MessageHandler msgHandler(this, &readBuffer, length);
    msgHandler.handleMessage();
}


/**
 * @brief writes a n-size message to the buffer on specific descriptor
 * 
 * @param buffer - pointer to our buffer
 * @param count - amount of bytes we want to send 
*/

void NetworkAgent::write(char * buffer, int count) {
    if(dataToWrite.size() != 0) {
        dataToWrite.emplace_back(buffer, count);
        return;
    }
    int sent = send(_fd, buffer, count, MSG_DONTWAIT);
    if(sent == count)
        return;
    if(sent == -1) {
        if(errno != EWOULDBLOCK && errno != EAGAIN){
            // ! Handle error
            return;
        }
        dataToWrite.emplace_back(buffer, count);
    } else {
        dataToWrite.emplace_back(buffer+sent, count-sent);
    }
    waitForWrite(true);
};


void NetworkAgent::waitForWrite(bool epollout) {
    epoll_event ee {EPOLLIN|EPOLLRDHUP|(epollout?EPOLLOUT:0), {.ptr=this}};
    epoll_ctl(epollFd, EPOLL_CTL_MOD, _fd, &ee);
}
