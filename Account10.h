#pragma once

#include <string>

class Account10 {
public:
    /* 
        If username starts with GDUsrXXXX, 
        where XXXX is a number,  this user
        was actually renamed automatically
    */
    bool autoRenamed; 
    std::string username;
    int userID;

    //virtual void parseFromString(std::string server_response);
};