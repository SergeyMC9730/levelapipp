#pragma once

#include <string>

class CURLParameter {
public:
    CURLParameter();
    CURLParameter(std::string k, std::string v);
    CURLParameter(std::string k, int v);

    std::string key;
    std::string value;
};
