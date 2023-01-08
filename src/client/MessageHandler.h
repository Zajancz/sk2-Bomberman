#include <cstdlib>
#include <cstdio>
#include "../server/Messages/Text.h"
#include "../server/Client.h"
#include "../server/MessageHandler.h"
#include "../server/Buffer.h"
// #include "NetworkAgent.h"

namespace Client {
    
    /* 
    In future, as a refacotr, we can make an abstract class,
    that would work as an interface, for this class, and it's
    server side equivalent.
    This would allow us to move certain methods (prob. as templates),
    to a common area, used both by the client and server.
    */
    // This is a Client - side message handler, base functionality is inherited from the server
    class MessageHandler {//: Server::MessageHandler {
        Server::Client * client; // Client from whom the mesasge is
        Buffer * readBuffer; // Clients buffer
        int length; // message length
    public:
        MessageHandler(Server::Client* _client, Buffer* _buffer, int _length);
    //     ~MessageHandler();

        template<class Message>
        int resolveType();

        template<class Message>
        void sendMessage(Message message);

        // Resolves message type and calls corresponding method
        void handleMessage();
        
    //     // Writes a message to console
    //     void handleTextType();
        
    //     void handlePlayerPositionType();

    //     void handleAllPlayersPositionsType();
    };
};