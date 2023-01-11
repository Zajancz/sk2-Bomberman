#include "MessageHandler.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include "GameManager.h"

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
        case 3: handleEnemiesPositionsType(); break;
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
void MessageHandler::handleEnemiesPositionsType(){
    printf("Client::MessageHandler::handleEnemiesPositions\n");
    AllPlayersPositions enemies;
    memcpy(&enemies, &readBuffer->data[4], sizeof(AllPlayersPositions));
    for (int i = 0; i < sizeof(enemies.ids) / sizeof(int); i++) {
        if (enemies.ids[i] == 0) break;
        printf("playerId: %d, position: {%d,%d}\n",
            enemies.ids[i],
            enemies.position[i].x,
            enemies.position[i].y);
        GameManager::addEnemy(enemies.ids[i], enemies.position[i]);
    }
};