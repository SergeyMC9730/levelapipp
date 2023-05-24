#pragma once

#include <string>

class Account10 {
public:
    /* 
        if username starts with GDUsrXXXX, 
        where XXXX is a number,  this user
        was actually renamed automatically
    */
    bool autoRenamed; 
    std::string username;
    int userID;
};