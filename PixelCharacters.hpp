#pragma once

#include <map>
#include <vector>
#include <string>

namespace LevelAPI {
    extern std::map<char, std::vector<int>> characters;

    void drawOnImage(std::string str, void *img, int x, int y);
};