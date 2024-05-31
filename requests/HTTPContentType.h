#pragma once

#include <string>

class HTTPContentType {
protected:
    std::string m_sType;
public:
    HTTPContentType();
    
    std::string getType();
};