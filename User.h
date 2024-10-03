#pragma once
#include <string>

class User
{
    std::string _login;
    std::string _password;
    std::string _name;

public:
    User(const std::string& login, const std::string& password, const std::string& name) :
        _login(login),
        _password(password),
        _name(name)
    {}

    User(const std::string& login, const std::string& password) :
        _login(login),
        _password(password)
    {}
};
