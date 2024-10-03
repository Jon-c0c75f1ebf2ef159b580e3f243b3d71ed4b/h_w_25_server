#include "Database.h"
#include <iostream>
#include <string>

bool Database::connectDatabase(
    const std::string& host,
    const std::string& user,
    const std::string& password,
    const std::string& dbName)
{
    mysql_init(&mysql);

    if (&mysql == nullptr) {
        std::cout << "Error: can't create MySQL-descriptor" << std::endl;
        exit(1);
    }

    if (!mysql_real_connect(&mysql, host.c_str(), user.c_str(), password.c_str(), dbName.c_str(), 0, nullptr, 0)) {
        std::cout << "Error: can't connect to database " << mysql_error(&mysql) << std::endl;
        return false;
    }
    else {
        std::cout << "Success!" << std::endl;
    }

    return true;
}


void Database::disconnect() {
    mysql_close(&mysql);
}

bool Database::checkLogin(std::string login, std::string password) {
    std::string query = "SELECT passwords.password_hash FROM users "
        "JOIN passwords ON users.user_id = passwords.user_id "
        "WHERE users.login = '" + login + "'";

    if (mysql_query(&mysql, query.c_str()) == 0) {
        res = mysql_store_result(&mysql);
        if (mysql_fetch_row(res) != nullptr) {
            mysql_free_result(res);
            return true;
        }
    }

    return false;
}


std::string Database::getName(std::string login) {
    std::string getName = "SELECT name FROM users "
        "WHERE login = '" + login + "'";

    std::string name;

    if (mysql_query(&mysql, getName.c_str()) == 0) {
        res = mysql_store_result(&mysql);
        if (row = mysql_fetch_row(res)) {
            name = row[0];
            mysql_free_result(res);

            return name;
        }
    }

    return "";
}


std::string Database::checkRegister(std::string login, std::string name, std::string password) {
    std::string getLogin = "SELECT login FROM users "
        "WHERE login = '" + login + "'";

    std::string getName = "SELECT name FROM users "
        "WHERE name = '" + name + "'";

    if (mysql_query(&mysql, getLogin.c_str()) == 0) {
        res = mysql_store_result(&mysql);
        if (mysql_fetch_row(res) != nullptr) {
            mysql_free_result(res);
            return login;
        }
    }
    else if (mysql_query(&mysql, getName.c_str()) == 0) {
        res = mysql_store_result(&mysql);
        if (mysql_fetch_row(res) != nullptr) {
            mysql_free_result(res);
            return name;
        }
    }

    return "";
}


bool Database::registerUser(std::string login, std::string name, std::string password) {
    std::string registerUserQuery = "INSERT INTO users (name, login) "
        "VALUES ('" + name + "', '" + login + "')";

    std::string addUserPasswordQuery =
        "INSERT INTO passwords (user_id,password_hash)\n"
        "VALUES ((SELECT LAST_INSERT_ID()), '" + password + "')";

    if (mysql_query(&mysql, registerUserQuery.c_str()) == 0 &&
        mysql_query(&mysql, addUserPasswordQuery.c_str()) == 0) {
        return true;
    }

    return false;
}

bool Database::addMessage(std::string from, std::string to, std::string message) {
    std::string getFromId = "SELECT user_id FROM users WHERE name = '" + from + "'";
    if (mysql_query(&mysql, getFromId.c_str()) == 0) {
        res = mysql_store_result(&mysql);
        if (row = mysql_fetch_row(res)) {
            int fromId = atoi(row[0]);
            mysql_free_result(res);

            std::string getToId = "SELECT user_id FROM users WHERE name = '" + to + "'";
            if (mysql_query(&mysql, getToId.c_str()) == 0) {
                res = mysql_store_result(&mysql);
                if (row = mysql_fetch_row(res)) {
                    int toId = atoi(row[0]);
                    mysql_free_result(res);

                    std::string addMsg =
                        "INSERT INTO messages (from_id, to_id, message) "
                        "VALUES ('" + to_string(fromId) + "', '" + to_string(toId) + "', '" + message + "')";
                    if (mysql_query(&mysql, addMsg.c_str()) == 0) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}


std::vector<std::string> Database::getAllMessages(std::string name) {
    std::string getAllMessages = "SELECT u1.name AS from_name, u2.name AS to_name, m.message "
        "FROM messages m "
        "JOIN users u1 ON u1.user_id = m.from_id "
        "JOIN users u2 ON u2.user_id = m.to_id "
        "WHERE(u1.name = '" + name + "' OR u2.name = '" + name + "') OR m.to_id = 1";

    std::vector<std::string> resultVector;

    if (mysql_query(&mysql, getAllMessages.c_str()) == 0) {
        res = mysql_store_result(&mysql);
        resultVector = convertResToVector(res);
        mysql_free_result(res);

        return resultVector;
    }

    return std::vector<std::string>();
}


std::vector<std::string> Database::getAllUsers() {
    std::string getAllUsers = "SELECT name FROM users";
    std::vector<std::string> resultVector;

    if (mysql_query(&mysql, getAllUsers.c_str()) == 0) {
        res = mysql_store_result(&mysql);
        resultVector = convertResToVector(res);
        mysql_free_result(res);

        return resultVector;
    }

    return std::vector<std::string>();
}


std::vector<std::string> Database::convertResToVector(MYSQL_RES* res) {
    std::vector<std::string> resultVector;

    while (row = mysql_fetch_row(res)) {
        for (int i = 0; i < mysql_num_fields(res); i++) {
            if (row[i] != nullptr) {
                resultVector.push_back(std::string(row[i]));
            }
        }
    }

    return resultVector;
}
