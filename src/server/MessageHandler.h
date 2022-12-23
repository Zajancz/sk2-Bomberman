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
#include "Buffer.h"
#include "Client.h"

class MessageHandler {
private:
    Client * client; // Client from whom the mesasge is
    Buffer * readBuffer; // Clients buffer
    int lenth; // message length
public:
    MessageHandler(Client*, Buffer*, int);
    virtual ~MessageHandler();

    void handleMessage(); // Resolves message type and calls corresponding method
    
    void handleTextType(); // Writes a message to console
    // In future, it will convert from binary to a structure, then write message to console
    
    void handlePlayerPositionType();
    void handleRequestNewBomb();
};