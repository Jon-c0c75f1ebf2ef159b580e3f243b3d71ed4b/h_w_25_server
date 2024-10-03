#include "Server.h"

void Server::createSocket()
{
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "Winsock initialization error!" << std::endl;
        exit(1);
    }

    sockert_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (sockert_file_descriptor == INVALID_SOCKET) {
        std::cout << "Creation of Socket failed!" << std::endl;
        exit(1);
    }
    else {
        std::cout << "Socket creation was successful!" << std::endl;
    }
#else
    sockert_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (sockert_file_descriptor == -1) {
        std::cout << "Creation of Socket failed!" << std::endl;
        exit(1);
    }
    else {
        std::cout << "Socket creation was successful!" << std::endl;
    }
#endif
}

void Server::bindSocket()
{
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_family = AF_INET;
    bind_status = bind(sockert_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if (bind_status == -1) {
        std::cout << "Socket binding failed.!" << std::endl;
        exit(1);
    }
    else {
        std::cout << "The socket binding was successful!" << std::endl;
    }
}

void Server::closeSocket()
{
#ifdef _WIN32
    closesocket(sockert_file_descriptor);
    WSACleanup();
#else
    close(sockert_file_descriptor);
#endif 
}

void Server::dataReceivingServer()
{
    connection_status = listen(sockert_file_descriptor, 5);

    if (connection_status == -1) {
        std::cout << "Socket is unable to listen for new connections!" << std::endl;
        exit(1);
    }
    else {
        std::cout << "Server is listening for new connection: " << std::endl;
    }

    length = sizeof(client);
    connection = accept(sockert_file_descriptor, (struct sockaddr*)&client, &length);

    if (connection == -1) {
        std::cout << "Server is unable to accept the data from client!" << std::endl;
        exit(1);
    }
    else {
        std::cout << "The client has joined!" << std::endl;
    }
}

char* Server::readData()
{

#ifdef _WIN32
    memset(message, '\0', MESSAGE_LENGTH);

    recv(connection, message, sizeof(message), 0);
#else
    bzero(message, MESSAGE_LENGTH);

    read(connection, message, sizeof(message));
#endif

    std::cout << "Data received from client: " << message << std::endl;
    return message;
}

void Server::writeData(std::string serverMessage)
{
#ifdef _WIN32
    memset(message, '\0', MESSAGE_LENGTH);
    strcpy_s(message, serverMessage.c_str());

    int bytesSent = send(connection, message, sizeof(message), 0);

    if (bytesSent >= 0) {
        std::cout << "Data successfully sent to the client.!" << message << std::endl;
    }
#else
    bzero(message, MESSAGE_LENGTH);
    strcpy_s(message, serverMessage.c_str());

    ssize_t bytes = write(connection, message, sizeof(message));

    if (bytes >= 0) {
        std::cout << "Data successfully sent to the client.!" << message << std::endl;
    }
#endif
}
