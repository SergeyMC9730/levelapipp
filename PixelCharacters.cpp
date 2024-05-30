#include "PixelCharacters.hpp"
//static std::map<char, std::vector<bool>> characters;

std::map<char, std::vector<int>> LevelAPI::characters = {
    {'0', {
        1, 1, 1,
        1, 0, 1,
        1, 0, 1,
        1, 0, 1,
        1, 1, 1
    }},
    {'1', {
        0, 0, 1,
        0, 1, 1,
        1, 0, 1,
        0, 0, 1,
        0, 0, 1
    }},
    {'2', {
        1, 1, 1,
        0, 0, 1,
        1, 1, 1,
        1, 0, 0,
        1, 1, 1
    }},
    {'3', {
        1, 1, 1,
        0, 0, 1,
        1, 1, 1,
        0, 0, 1,
        1, 1, 1
    }},
    {'4', {
        1, 0, 1,
        1, 0, 1,
        1, 1, 1,
        0, 0, 1,
        0, 0, 1
    }},
    {'5', {
        1, 0, 1,
        1, 0, 0,
        1, 1, 1,
        0, 0, 1,
        1, 1, 1
    }},
    {'6', {
        1, 1, 1,
        1, 0, 0,
        1, 1, 1,
        1, 0, 1,
        1, 1, 1
    }},
    {'7', {
        1, 1, 1,
        0, 0, 1,
        0, 0, 1,
        0, 0, 1,
        0, 0, 1
    }},
    {'8', {
        1, 1, 1,
        1, 0, 1,
        1, 1, 1,
        1, 0, 1,
        1, 1, 1
    }},
    {'9', {
        1, 1, 1,
        1, 0, 1,
        1, 1, 1,
        0, 0, 1,
        1, 1, 1
    }},
    {'m', {
        1, 0, 1,
        1, 1, 1,
        1, 1, 1,
        1, 0, 1,
        1, 0, 1
    }},
    {'a', {
        1, 1, 1,
        1, 0, 1,
        1, 1, 1,
        1, 0, 1,
        1, 0, 1
    }},
    {'x', {
        1, 0, 1,
        1, 0, 1,
        0, 1, 0,
        1, 0, 1,
        1, 0, 1
    }},
    {' ', {
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        0, 0, 0
    }},
    {':', {
        0, 0, 0,
        0, 1, 0,
        0, 0, 0,
        0, 1, 0,
        0, 0, 0
    }}
};

#include "raylib/src/raylib.h"

namespace LevelAPI {
    Vector2 drawPixelOnImage(Image *img, int _x, int _y, int size, Color col) {
        if (size == 1) {
            ImageDrawPixel(img, _x, _y, col);

            return {1, 1};
        }

        ImageDrawRectangle(img, _x, _y, size, size, col);

        return {size, size};
    }
}

void LevelAPI::drawOnImage(std::string str, void *img_, int _x, int _y) {
    Image *img = (Image *)img_;
    
    int ax = _x;
    int ay = _y;

    int size = 4;
    
    for (int i = 0; i < str.size(); i++) {
        if (characters.count(str[i])) {
            std::vector<int> pixel_data = characters[str[i]];

            for (int x = 0; x < 3; x++) {
                for (int y = 4; y > -1; y--) {
                    int index = (y * 3) + x;

                    int val = pixel_data[index];

                    if (val != 0) drawPixelOnImage(img, ax + (x * size), ay + (y * size), size, BLACK);
                }
            }

            ax += (3 * size) + size;
        }
        if (str[i] == '\n') {
            ay += (5 * size) + size;
        }
    }
}
