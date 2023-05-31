#pragma once

#include <string>

#include "RobTopStringContainer.hpp"

class Account10 {
protected:
    RobTopStringContainer *m_pContainer;

    virtual void addParsers();
public:
    /* 
        If username starts with GDUsrXXXX, 
        where XXXX is a number,  this user
        was actually renamed automatically
    */
    bool autoRenamed; 
    std::string username;
    int userID;

    Account10();
    ~Account10();

    virtual void parseFromString(std::string server_response);
};