#include "hex.h"

std::string convertHexToStr(std::string str) {
    int len = str.length();
    int i = 0;

    std::string newString;
    
    while(i < len) {
        std::string byte = str.substr(i, 2);
        char chr = (char)((int)strtol(byte.c_str(), NULL, 16));
        newString.push_back(chr);

        i += 2;
    }

    return newString;
}