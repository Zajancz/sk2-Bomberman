#include "MessageHandler.h"

void MessageHandler::handleMessage() 
{
    if(length < 4) {
        printf("Error message. Example: 0001 (text)\n");

        Text text = {"Testing message","second field"};
        sendMessage<Text>(text);

    } else {
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
template<class Message>
void MessageHandler::sendMessage(Message message) {
    // converting message to char *: "0000<data>\n" format
    char converted[sizeof(message) + 5] = "0001"; // TODO: dynamically get the message type id
    converted[sizeof(converted) - 1] = '\n';
    memcpy(&converted[4], &message, sizeof(message));
    client->write(converted, sizeof(converted));
    
    printf("Sending message of size: sizeof %ld, Encoded: %s.\n", sizeof(converted), converted);
    printf("Encoded message (with separated characters):\n");
    for (int i = 0; i < sizeof(converted); i++) {
        printf("%c-",converted[i]);
    }
    printf("\n");

    // testing reading the message from converted
    Text text2;
    memcpy(&text2, &converted[4], sizeof(Text));
    printf("test reading: %s,%s\n",text2.content1,text2.content2);
}