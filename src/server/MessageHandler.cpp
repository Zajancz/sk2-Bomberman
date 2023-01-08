#include "MessageHandler.h"

using namespace Server;

MessageHandler::MessageHandler(Client* _client, Buffer* _buffer, int _length) {
    client = _client;
    readBuffer = _buffer;
    length = _length;
}

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
        // case 3: handleRequestNewBomb(); break;
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
    printf("Encoded message (with separated characters):\n");
    for (int i = 0; i < sizeof(converted); i++) {
        printf("%c-",converted[i]);
    }
    printf("\n");
}
template<class Message>
int MessageHandler::resolveType() {
    if (typeid(Message) == typeid(Text)) return 1;
    if (typeid(Message) == typeid(PlayerPosition)) return 2;
    if (typeid(Message) == typeid(AllPlayersPositions)) return 3; // ! temporarily 3
    // TODO ...
    
    return 0;
}

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
void MessageHandler::handlePlayerPositionType(){
    printf("handlePlayerPosition, responding with AllPlayersPositons\n");
    PlayerPosition position;
    memcpy(&position, &readBuffer->data[4], sizeof(PlayerPosition));
    printf("received PlayerPosition: {%d,%d}\n",position.x,position.y);
    client->gameManager->updatePlayerPosition(client->fd(), position);
    printf("Updated position on the server\n");
    client->gameManager->getPlayersPositions();
    printf("Ready to send all positions\n");
};