#include "MessageHandler.h"

using namespace Server;

MessageHandler::MessageHandler(Client* _client, Buffer* _buffer, int _length) {
    client = _client;
    readBuffer = _buffer;
    length = _length;
}

/**
 * @brief This function is used to identify a message type and call a specific function 
 * 
 * For example, if in the buffer will be 0001, then we know the client sends to the server
 * a text message, nothing more.
 * 
*/

void MessageHandler::handleMessage() 
{
    if(length < 4) {
        printf("Error message. Example: 0001 (text)\n");
        Text text = {"Testing \nmessage",0,"yyy"};
        sendMessage<Text>(text);
    } else {
        std::string str(readBuffer->data, 0, 4);
        int typeMessage = std::stoi(str);
        switch (typeMessage) {
        case 1: handleTextType(); break;
        case 2: handlePlayerPositionType(); break;
        // case 3: handleAllPlayersPositionsType(); break;
        case 4: handleBombType(); break;
        // case 4: handleRequestNewUser(); break;
        // case 5: handleRequestNewLobby(); break;
        // case 6: handleRequestLobbyList(); break;
        // case 7: handleRequestJoin(); break;
        // case 8: handleRequestReady(); break;    
        default:
            printf("Wrong Type\n");
            break;
        }
    }
}

/**
 * @brief This function is used to send a message between server and clients
 * 
 * @param message we provide here a message from the buffer
 * 
*/

template<class Message>
void MessageHandler::sendMessage(Message message) {
    // converting message to char *: "0000<data>\n" format
    char converted[sizeof(message) + 5] = "0001";
    int type = resolveType<Message>();
    sprintf(converted, "%4d", type);
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

/**
 * @brief This function is used to resolve type of a message
 * 
 * This function mainly used by sendMessage function
 * 
 * @return returns basic 1/2/3/4 based of type read from the buffer
*/

template<class Message>
int MessageHandler::resolveType() {
    if (typeid(Message) == typeid(Text)) return 1;
    if (typeid(Message) == typeid(PlayerPosition)) return 2;
    if (typeid(Message) == typeid(AllPlayersPositions)) return 3;
    if (typeid(Message) == typeid(Bomb)) return 4;
    // TODO ...
    
    return 0;
}

/**
 * @brief This function called, when we read 0001 from the buffer
 * 
 * Such function used to get current position of a player
 * 
*/

void MessageHandler::handleTextType() 
{
    Text text;
    memcpy(&text, &readBuffer->data[4], sizeof(Text));
    printf("received Text: {%s,%d,%s}\n",text.content1,text.content2,text.content3);

    printf("sending a position\n");
    PlayerPosition position {44, 55};
    sendMessage<PlayerPosition>(position);
    printf("sent a position\n");
}

/**
 * @brief This function called, when we read 0002 from the buffer
 * 
 * Such function used to update current position of a player or ememies
 * 
*/

void MessageHandler::handlePlayerPositionType(){
    printf("handlePlayerPosition, responding with AllPlayersPositons containing enemies\n");
    PlayerPosition position;
    memcpy(&position, &readBuffer->data[4], sizeof(PlayerPosition));
    printf("received PlayerPosition: {%d,%d}\n",position.x,position.y);
    client->gameManager->updatePlayerPosition(client->fd(), position);
    printf("Updated position on the server\n");
    AllPlayersPositions app = client->gameManager->getEnemiesPositions(client->fd());
    sendMessage<AllPlayersPositions>(app);
    printf("Sent positions of enemies for player %d\n", client->fd());
    while (true) {
        Bomb bomb = client->gameManager->getBomb(client->fd());
        if (bomb.fd == 0) break;
        sendMessage<Bomb>(bomb);
        printf("Sent new bomb {%d, {%d, %d}} to client", bomb.fd, bomb.position.x, bomb.position.y);
    }
};

/**
 * @brief This function called, when we read 0003 from the buffer
 * 
 * Such function used to place bomb on the map by current player in a specific location
 * 
*/

void MessageHandler::handleBombType(){
    printf("handleBomb, adding new bomb to the game\n");
    Bomb bomb;
    memcpy(&bomb, &readBuffer->data[4], sizeof(Bomb));
    // new bomb does not contain fd, inserting it now:
    bomb.fd = client->fd();
    printf("received Bomb: {%d {%d,%d}}\n",bomb.fd, bomb.position.x, bomb.position.y);
    client->gameManager->addBomb(bomb);
    printf("Added new bomb on the server\n");
};