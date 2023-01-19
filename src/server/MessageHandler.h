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
#include "GameManager.h"
#include "Messages/Text.h"

#include <string>

namespace Server {
    class MessageHandler {
    private:
        Client * client; // Client from whom the mesasge is
        Buffer * readBuffer; // Clients buffer
        int length; // message length
    public:
        MessageHandler();
        MessageHandler(Client* _client, Buffer* _buffer, int _length);
        
        virtual ~MessageHandler(){ }

        template<class Message>
        void sendMessage(Message message);

        template<class Message>
        int resolveType();

        // Resolves message type and calls corresponding method
        void handleMessage();
        
        // Writes a message to console
        void handleTextType();
        
        void handlePlayerPositionType();

        void handleBombType();

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
}