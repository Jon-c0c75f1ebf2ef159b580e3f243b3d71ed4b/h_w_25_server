//skillfactory home work modul 25 Server
//h_w_25_server
#include <iostream>

#include "Server.h"
#include "Chat.h"

int main()
{
    std::string serverMessage;
    std::string clientMessage;

    Server server;
    Chat chat;

    server.createSocket();
    server.bindSocket();
    server.dataReceivingServer();

    while (true) {
        clientMessage = server.readData();
        if (clientMessage == "exit") {
            break;
        }
        serverMessage = chat.messageProcessing(clientMessage);
        server.writeData(serverMessage);
    }

    server.closeSocket();

    return 0;
}
