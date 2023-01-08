#include "MessageHandler.h"
#include <iostream>
#include <string>
#include <stdio.h>

using namespace Client;

MessageHandler::MessageHandler(NetworkAgent* _client, Buffer* _buffer, int _length) {
    client = _client;
    readBuffer = _buffer;
    length = _length;
}

void MessageHandler::handleMessage() {
    if(length < 4) {
        printf("Error message. Example: 0001 (text)\n");
        Text text = {"Testing \nmessage",0,"yyy"};
        sendMessage<Text>(text);
    } else {
        std::string str(readBuffer->data, 0, 4);
        std::cout<<"MessageType:{"<< str<<"}\n";
        int typeMessage = std::stoi(str);
        switch (typeMessage) {
        case 1: handleTextType(); break;
        case 2: handlePlayerPositionType(); break;
        case 3: handleAllPlayersPositionsType(); break;
        // TODO ... 
        default:
            printf("Wrong Type\n");
            break;
        }
    }
}

void MessageHandler::handleTextType() 
{
    Text text;
    memcpy(&text, &readBuffer->data[4], sizeof(Text));
    printf("Client::MessageHandler::handleText received Text: {%s,%d,%s}\n",text.content1,text.content2,text.content3);

    printf("sending a position\n");
    PlayerPosition position {44, 55};
    sendMessage<PlayerPosition>(position);
    printf("sent a position\n");
}
void MessageHandler::handlePlayerPositionType(){
    printf("Client::MessageHandler::handlePlayerPosition\n");
};
void MessageHandler::handleAllPlayersPositionsType(){
    printf("Client::MessageHandler::handleAllPlayersPositions\n");
};