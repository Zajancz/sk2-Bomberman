#pragma once

#include <cstdlib>
#include <cstdio>
#include "../server/Messages/Text.h"
#include "../server/Client.h"
#include "../server/MessageHandler.h"
#include "../server/Buffer.h"
#include "NetworkAgent.h"
#include "../game/Player.h"
#include "../game/Bomb.h"

namespace Client {
    
    /* 
    In future, as a refactor, we can make an abstract class,
    that would work as an interface, for this class, and it's
    server side equivalent.
    This would allow us to move certain methods (prob. as templates),
    to a common area, used both by the client and server.
    */
    // This is a Client - side message handler, base functionality is inherited from the server
    class MessageHandler {//: Server::MessageHandler {
        NetworkAgent * client; // Client from whom the message is
        Buffer * readBuffer; // Clients buffer
        int length; // Message length
    public:
        MessageHandler(NetworkAgent* _client, Buffer* _buffer, int _length);
    //     ~MessageHandler();

        // Resolves message type and calls corresponding method
        void handleMessage();

        //? Template methods need to be defined in header files
        //? https://stackoverflow.com/questions/20461146/undefined-reference-when-using-templates
        //? to check for undefined references use the command: $ ld godot/bin/linux/libsimple.so
        // Resolves a structure type, returns its identifier as int
        template<class Message>
        int resolveType() {
            if (typeid(Message) == typeid(Text)) return 1;
            if (typeid(Message) == typeid(PlayerPosition)) return 2;
            if (typeid(Message) == typeid(AllPlayersPositions)) return 3;
            if (typeid(Message) == typeid(Bomb)) return 4;
            // TODO ...
            return 0;
        }

        //? Template methods need to be defined in header files
        //? https://stackoverflow.com/questions/20461146/undefined-reference-when-using-templates
        //? to check for undefined refernces use the command: $ ld godot/bin/linux/libsimple.so
        // Writes a message to outgoing buffer, depends on: this:resolveType, this:client:write;
        template<class Message>
        void sendMessage(Message message) {
            // converting message to char *: "0000<data>\n" format
            char converted[sizeof(message) + 5];
            int type = resolveType<Message>();
            sprintf(converted, "%4d", type); // assigns a type number to first 4 bytes
            converted[sizeof(converted) - 1] = '\n';
            memcpy(&converted[4], &message, sizeof(message));
            client->write(converted, sizeof(converted));
            
            printf("Sending message of size: sizeof %ld, Encoded: %s.\n", sizeof(converted), converted);
            // printf("Encoded message (with separated characters):\n");
            // for (int i = 0; i < sizeof(converted); i++) {
            //     printf("%c-",converted[i]);
            // }
            // printf("\n");
        }

        // Writes a message to console
        void handleTextType();
        
        void handlePlayerPositionType();

        void handleEnemiesPositionsType();

        void handleBombType();
    };
};