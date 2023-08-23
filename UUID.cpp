/**
 *  LevelAPI - Geometry Dash level cacher with search functionality and more.
    Copyright (C) 2023  Sergei Baigerov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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