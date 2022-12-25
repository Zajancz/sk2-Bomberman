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

    // Resolves message type and calls corresponding method

    void handleMessage() 
    {
        if(length < 4)
            printf("Error message. Example: 0001 (text)\n");
        else
        {
            std::string str(readBuffer->data, 0, 4);
            int typeMessage = std::stoi(str);

            switch (typeMessage) {
            case 1:
                handleTextType();
                break;
            case 2:
                handlePlayerPositionType();
                break;
            case 3:
                handleRequestNewBomb();
                break;
            case 4:
                handleRequestNewUser();
                break;
            case 5:
                handleRequestNewLobby();
                break;
            case 6:
                handleRequestLobbyList();
                break;
            case 7:
                handleRequestJoin();
                break;
            case 8:
                handleRequestReady();
                break;    
            default:
                printf("Wrong Type\n");
                break;
            }
        }
    }
    
    // Writes a message to console
    // In future, it will convert from binary to a structure, then write message to console

    void handleTextType() 
    {
        printf(Text().content1);
    }
    
    
    void handlePlayerPositionType();
    void handleRequestNewBomb();
    void handleRequestNewUser();
    void handleRequestNewLobby();
    void handleRequestLobbyList();
    void handleRequestJoin();
    void handleRequestReady();
};
