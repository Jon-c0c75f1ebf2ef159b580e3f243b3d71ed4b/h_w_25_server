#pragma once
#include <vector>
#include <exception>
#include <memory>
#include "Message.h"
#include "User.h"

class Chat {
public:
    std::string messageProcessing(std::string clientMessage);

private:
    std::vector<std::string> _vectorFromMessage;

    std::string loginUser();
    std::string registerUser();
    std::string showChat();
    std::string addMessage();

    std::vector<std::string> messageToVector(std::string message, std::string delimiter);
};
