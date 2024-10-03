#pragma once
#include <string>

class Message
{
    std::string _from;
    std::string _to;
    std::string _text;

public:
    Message(const std::string& from, const std::string& to, const std::string& text) :
        _from(from),
        _to(to),
        _text(text)
    {}
};
