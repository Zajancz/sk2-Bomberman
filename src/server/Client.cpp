#include "Client.h"
#include "MessageHandler.h"

using namespace Server;

std::unordered_set<Server::Client*> Server::Client::clients;
int Server::Client::epollFd;

void Client::waitForWrite(bool epollout) {
    epoll_event ee {EPOLLIN|EPOLLRDHUP|(epollout?EPOLLOUT:0), {.ptr=this}};
    epoll_ctl(epollFd, EPOLL_CTL_MOD, _fd, &ee);
}
Client::Client(int fd) : _fd(fd) {
    printf("creating a new client\n");
    printf("epollFd is set to: %d\n", epollFd);
    epoll_event ee {EPOLLIN|EPOLLRDHUP, {.ptr=this}};
    epoll_ctl(epollFd, EPOLL_CTL_ADD, _fd, &ee);
    printf("created a new client\n");
    clients.insert(this);
}
Client::~Client(){
    epoll_ctl(epollFd, EPOLL_CTL_DEL, _fd, nullptr);
    shutdown(_fd, SHUT_RDWR);
    close(_fd);
}
int Client::fd() const {return _fd;}



/**
 * @brief This function is used to identify length of a message
 * 
 * @param length our message, which must be more than 4, because first 4 bytes of the buffer is used for
 * recognizing a message type
 * 
 * @return returns length of a full message of recognized type or -1 if unrecognized type
*/

int Client::expectedLength(int length) {
    int overhead = 5;
    if (length < 4) return -1;
    std::string str(readBuffer.data, 0, 4);
    int typeMessage = std::stoi(str);
    switch (typeMessage) {
        case 1: return sizeof(Text) + overhead;
        case 2: return sizeof(PlayerPosition) + overhead;
        case 3: return sizeof(AllPlayersPositions) + overhead;
        case 4: return sizeof(Bomb) + overhead;
        // TODO: ...
        default:
            printf("Client::expectedLength: Warning - Unknown type, can't read expected size");
            return -1;
    }
}

/**
 * @brief This function reads a message from the buffer
 * 
 * As we can see if some input event happened on EPOLLIN, then we read bytes from 
 * our buffer and checks for loss in transition. Also in this process we continiously update 
 * our current location in the buffer and send a message to be processed by another function
 * 
 * @param events - stores events, on which epoll was waiting
 * 
*/


void Client::handleEventEpollin(uint32_t events) 
{
    ssize_t count = read(_fd, readBuffer.dataPos(), readBuffer.remaining());
    if(count <= 0)
        events |= EPOLLERR;
    else {
        readBuffer.pos += count;
        // Find '\n' symbol and return pointer to it's location (nullptr if not found)
        char * eol = (char*) memchr(readBuffer.data, '\n', readBuffer.pos);
        if(eol == nullptr) { // readBuffer does not contain `\n` symbol
            if(0 == readBuffer.remaining())
                readBuffer.doube();
        } else { // readbuffer contains at least one `\n`
            do {
                // determine if the message is received entirely
                auto available = &readBuffer.data[readBuffer.pos] - readBuffer.data + 1;
                int expected = expectedLength(available);
                printf("%ld/%d\n", available, expected);
                if (expected == -1 || available < expected) break;
                auto thismsglen = expected;
                // handleMessage(thismsglen);
                printf("Handling client's message\n");
                MessageHandler msgHandler(this, &readBuffer, thismsglen);
                msgHandler.handleMessage();
                // #### Prepare for next message
                // calculate lenth of remaining data in buffer
                auto nextmsgslen =  readBuffer.pos - thismsglen;
                // update buffer position
                memmove(readBuffer.data, eol+1, nextmsgslen);
                readBuffer.pos = nextmsgslen;
            } while((eol = (char*) memchr(readBuffer.data, '\n', readBuffer.pos)));
        }

    }

    if(events & ~EPOLLIN) {
        remove();
    }

}

/**
 * @brief This function tests EPOLLOUT from the client side
 * 
 * @param events - stores events, on which epoll was waiting
*/

void Client::handleEventEpollout(uint32_t events) 
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

/**
 * @brief writes a n-size message to the buffer on specific descriptor
 * 
 * @param buffer - pointer to our buffer
 * @param count - amount of bytes we want to send 
*/

void Client::write(char * buffer, int count) {
    if(dataToWrite.size() != 0) {
        dataToWrite.emplace_back(buffer, count);
        return;
    }
    int sent = send(_fd, buffer, count, MSG_DONTWAIT);
    if(sent == count)
        return;
    if(sent == -1) {
        if(errno != EWOULDBLOCK && errno != EAGAIN){
            remove();
            return;
        }
        dataToWrite.emplace_back(buffer, count);
    } else {
        dataToWrite.emplace_back(buffer+sent, count-sent);
    }
    waitForWrite(true);
}

/**
 * @brief removes current client from the session
*/

void Client::remove() {
    printf("removing %d\n", _fd);
    clients.erase(this);
    delete this;
}


void Client::sendToAllBut(int fd, char * buffer, int count){
    auto it = clients.begin();
    while(it!=clients.end()){
        Client * client = *it;
        it++;
        if(client->fd()!=fd)
            client->write(buffer, count);
    }
}