#include <iostream>
#include <string>
#include "Chat.h"
#include "Database.h"

std::string Chat::messageProcessing(std::string clientMessage)
{
    std::string outMessage;

    _vectorFromMessage = messageToVector(clientMessage, ":");

    if (_vectorFromMessage[0] == "loginUser") {
        outMessage = loginUser();
    }
    else if (_vectorFromMessage[0] == "registerUser") {
        outMessage = registerUser();
    }
    else if (_vectorFromMessage[0] == "showChat") {
        outMessage = showChat();
    }
    else if (_vectorFromMessage[0] == "addMessage") {
        outMessage = addMessage();
    }
    else if (_vectorFromMessage[0] == "showUsers") {
        outMessage = showUsers();
    }

    return outMessage;
}

std::vector<std::string> Chat::messageToVector(std::string message, std::string delimiter)
{
    std::vector<std::string> resultVector;

    if (!message.empty()) {
        int start = 0;
        do {
            int idx = message.find(delimiter, start);
            if (idx == std::string::npos) {
                break;
            }

            int length = idx - start;
            resultVector.push_back(message.substr(start, length));
            start += (length + delimiter.size());
        } while (true);

        resultVector.push_back(message.substr(start));
    }

    return resultVector;
}

std::string Chat::loginUser()
{
    std::string login, password, name, outMessage;
    login = _vectorFromMessage[1];
    password = _vectorFromMessage[2];

    Database database{};
    database.connectDatabase(db_host, db_user, db_password, db_name);

    if (database.checkLogin(login, password)) {
        outMessage = "loginUser:ok:" + database.getName(login);
    }
    else {
        outMessage = "loginUser:error";
    }

    database.disconnect();

    return outMessage;
}

std::string Chat::registerUser()
{
    std::string login, password, name, outMessage;
    login = _vectorFromMessage[1];
    password = _vectorFromMessage[2];
    name = _vectorFromMessage[3];

    Database database{};
    database.connectDatabase(db_host, db_user, db_password, db_name);

    if (login == database.checkRegister(login, name, password) || login == "all") {
        outMessage = "registerUser:error:login";
        database.disconnect();
        return outMessage;
    }

    std::string nameExists = database.checkRegister(login, name, password);
    if (nameExists.empty()) {
        outMessage = "registerUser:error:name";
        database.disconnect();
        return outMessage;
    }

    if (database.registerUser(login, name, password)) {
        std::cout << "new registration: " << login << std::endl;
        outMessage = "registerUser:ok:" + name;
    }
    else {
        outMessage = "registerUser:error:registration_failed";
    }

    database.disconnect();

    return outMessage;

}

std::string Chat::addMessage()
{
    std::string from, to, text, outMessage;
    from = _vectorFromMessage[1];
    to = _vectorFromMessage[2];
    text = _vectorFromMessage[3];

    Database database{};
    database.connectDatabase(db_host, db_user, db_password, db_name);

    if (database.addMessage(from, to, text)) {
        outMessage = "addMessage:ok";
    }
    else {
        outMessage = "addMessage:error:" + to;
    }

    database.disconnect();

    return outMessage;
}

std::string Chat::showChat()
{
    std::string from, to, text, outMessage, name;
    name = _vectorFromMessage[1];

    Database database{};
    database.connectDatabase(db_host, db_user, db_password, db_name);

    std::vector<std::string> messages = database.getAllMessages(name);

    outMessage = "showChat:";

    for (const auto& message : messages) {
        outMessage += message + ":";
    }

    outMessage += "showChat";

    database.disconnect();

    return outMessage;
}

std::string Chat::showUsers()
{
    std::string name, outMessage;
    name = _vectorFromMessage[1];

    Database database{};
    database.connectDatabase(db_host, db_user, db_password, db_name);

    std::vector<std::string> users = database.getAllUsers();

    outMessage = "showUsers:";

    for (const auto& user : users) {
        if (user == name) {
            outMessage += user + "(ÿ):";
        }
        else {
            outMessage += user + ":";
        }
    }

    outMessage += "showUsers";

    database.disconnect();

    return outMessage;
}
