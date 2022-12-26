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
#include "Messages/Text.h"

#include <string>

class MessageHandler {
private:
    Client * client; // Client from whom the mesasge is
    Buffer * readBuffer; // Clients buffer
    int length; // message length
public:
    MessageHandler(Client* _client, Buffer* _buffer, int _length)
    {
        client = _client;
        readBuffer = _buffer;
        length = _length;
    }
    virtual ~MessageHandler(){ }

    template<class Message>
    void sendMessage(Message message);

    // Resolves message type and calls corresponding method
    void handleMessage();
    
    // Writes a message to console
    void handleTextType() 
    {
        Text text;
        memcpy(&text, &readBuffer->data[4], sizeof(Text));
        printf("received Text: {%s,%s}\n",text.content1,text.content2);
    }
    
    void handlePlayerPositionType(){
        printf("handlePlayerPositionType");
    };
    void handleRequestNewBomb(){
        printf("handleRequestNewBomb");
    };
    void handleRequestNewUser(){
        printf("handleRequestNewUser");
    };
    void handleRequestNewLobby(){
        printf("handleRequestNewLobby");
    };
    void handleRequestLobbyList(){
        printf("handleRequestLobbyList");
    };
    void handleRequestJoin(){
        printf("handleRequestJoin");
    };
    void handleRequestReady(){
        printf("handleRequestReady");
    };
};
