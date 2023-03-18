#include "StringSplit.h"

std::vector<std::string> splitString(const char *str, char d) {
    std::vector<std::string> result;

    do {
        const char *begin = str;

        while(*str != d && *str) str++;

        result.push_back(std::string(begin, str));
    } while (0 != *str++);

    return result;
}