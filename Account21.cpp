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

#include "Account21.h"
#include "RobTopStringContainer.hpp"

#include <stdexcept>

using namespace LevelAPI;

// add value parsers for robtop string (2.1)
void Account21::addParsers() {
    Account19::addParsers();

    // moderator level
    m_pContainer->setParserForVariable(
        49, 
        [&](std::string input, int id) {
            try {
                return std::stoi(input);
            } catch (std::invalid_argument &e) {
                return 0;
            }
        }
    );
}

// parse robtop string
void Account21::parseFromString(std::string server_response) {
    Account19::parseFromString(server_response);

    GET_KEY(m_pContainer, 49, this->modLevel, GKINT);
}