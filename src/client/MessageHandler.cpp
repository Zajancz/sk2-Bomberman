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

/**
 * @brief This function is used to identify a message type and call a specific function 
 * 
 * For example, if in the buffer will be 0001, then we know the client sends to the server
 * a text message, nothing more.
 * 
*/

void MessageHandler::handleMessage() {
    if(length < 4) {
        printf("Error message. Example: 0001 (text)\n");
        Text text = {"Testing \nmessage",0,"yyy"};
        sendMessage<Text>(text);
    } else {
        std::string str(readBuffer->data, 0, 4);
        // std::cout<<"MessageType:{"<< str<<"}\n";
        int typeMessage = std::stoi(str);
        switch (typeMessage) {
        case 1: handleTextType(); break;
        case 2: handlePlayerPositionType(); break;
        case 3: handleEnemiesPositionsType(); break;
        case 4: handleBombType(); break;
        // TODO ... 
        default:
            printf("ERROR: Wrong Type\n");
            break;
        }
    }
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
    printf("Client::MessageHandler::handleText received Text: {%s,%d,%s}\n",text.content1,text.content2,text.content3);

    printf("Sending a test position as a response\n");
    PlayerPosition position {44, 55};
    sendMessage<PlayerPosition>(position);
}

void MessageHandler::handlePlayerPositionType(){
    printf("Client::MessageHandler::handlePlayerPosition\n");
};

/**
 * @brief This function is used to get enemies position and send this information to Godot engine
*/

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

/**
 * @brief This function have data about bomb position, file descriptor, and send this information to Godot engine
*/

void MessageHandler::handleBombType()
{
    Bomb bomb;
    memcpy(&bomb, &readBuffer->data[4], sizeof(Bomb));
    printf("Client::MessageHandler::handlebomb received Bomb: {%d,{%d,%d}}\n", bomb.fd, bomb.position.x, bomb.position.y);
    GameManager::addBomb(bomb);
}