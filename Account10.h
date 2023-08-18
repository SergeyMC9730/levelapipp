#pragma once

#include <string>

#include "RobTopStringContainer.hpp"

class Account10 {
protected:
    // KV array
    RobTopStringContainer *m_pContainer;

    // adds parseres for our KV container
    virtual void addParsers();
public:
    /*
        If username starts with GDUsrXXXX,
        where XXXX is a number,  this user
        was actually renamed automatically
    */
    bool autoRenamed;
    // username
    std::string username;
    // user ID
    int userID;

    // constructor
    Account10();
    // destructor
    ~Account10();

    // parse userdata from string
    virtual void parseFromString(std::string server_response);
};
