#include "Client.h"
#include "MessageHandler.h"

class Client;

std::unordered_set<Client*> Client::clients;
int Client::epollFd;

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

void Client::handleEventEpollin(uint32_t events) 
{
    printf("Handling client's event\n");
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
                // #### Read message
                // length of data in buffer to closest \n 
                auto thismsglen = eol - readBuffer.data + 1;
                // save data to output buffer (dataToWrite)
                // only the length of this messag   
                sendToAllBut(_fd, readBuffer.data, thismsglen);  
                // TODO: MessageHandler //
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
void Client::remove() {
    printf("removing %d\n", _fd);
    clients.erase(this);
    delete this;
}
// This method is here only temporarily, it makes little sense here
// And we actually do not need it
void Client::sendToAllBut(int fd, char * buffer, int count){
    auto it = clients.begin();
    while(it!=clients.end()){
        Client * client = *it;
        it++;
        if(client->fd()!=fd)
            client->write(buffer, count);
    }
}
