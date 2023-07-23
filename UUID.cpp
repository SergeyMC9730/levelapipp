#include "UUID.h"
#include <vector>

using namespace LevelAPI::Backend;

std::string ConnectionCrypt::createUUID() {
    std::vector<std::string> table = {
        "0", "1", "2", "3",
        "4", "5", "6", "7",
        "8", "9", "a", "b",
        "c", "d", "e", "f"
    };
    int i = 0;
    std::string res = "";

    while(i < 8) {
        res += table.at(rand() % table.size());
        i++;
    }

    res += "-";
    i = 0;

    while(i < 4) {
        res += table.at(rand() % table.size());
        i++;
    }

    res += "-";
    i = 0;

    while(i < 4) {
        res += table.at(rand() % table.size());
        i++;
    }

    res += "-";
    i = 0;

    while(i < 4) {
        res += table.at(rand() % table.size());
        i++;
    }

    res += "-";
    i = 0;

    while(i < 10) {
        res += table.at(rand() % table.size());
        i++;
    }

    return res;
}