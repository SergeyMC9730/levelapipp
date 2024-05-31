/**
 *  LevelAPI - Geometry Dash level cacher with search functionality and more.
    Copyright (C) 2024  Sergei Baigerov

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

#include "tests.h"
#include "RobTopStringContainer.hpp"
#include <iostream>
#include "StringSplit.h"
#include "Translation.h"

using namespace LevelAPI;

void Tests::testRobtopParser() {
    std::cout << "testing robtop parser\n";

    RobTopStringContainer container;

    container.setParserForVariable(
        {
            52, 53
        },
        [&](std::string input, int id) {
            std::vector<int> vec;

            if (input.empty()) return vec;

            std::vector<std::string> vals = splitString(input.c_str(), ',');

            for (std::string _val : vals) {
                try {
                    vec.push_back(std::stoi(_val));
                } catch (std::invalid_argument e) {
                    std::cout << Frontend::Translation::getByKey("lapi.tslp.invalidargument") << std::endl;
                    continue;
                }
            }

            return vec;
        }
    );

    container.setString("52:1,2,3,4,5:53:1,2,3");
    container.parse();

    std::cout << "testing complete\n";
}