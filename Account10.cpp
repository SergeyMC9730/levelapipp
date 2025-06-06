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

#include "Account10.h"
#include "Account19.h"
#include <stdexcept>
#include "RobTopStringContainer.hpp"

using namespace LevelAPI;

// constructor
Account10::Account10() {
    m_pContainer = new RobTopStringContainer();
}

// destructor
Account10::~Account10() {
    delete m_pContainer;
}

// add all value parsers for robtop string
void Account10::addParsers() {
    // user id
    m_pContainer->setParserForVariable(
        1,
        [&](std::string input, int id, RobTopStringContainer*) {
            return input;
        }
    );

    // username
    m_pContainer->setParserForVariable(
        2,
        [&](std::string input, int id, RobTopStringContainer*) {
            try {
                return std::stoi(input);
            } catch (std::invalid_argument &e) {
                return 0;
            }
        }
    );
}

// parse account data from robtop string
void Account10::parseFromString(std::string server_response) {
    m_pContainer->setString(server_response);

    addParsers();

    m_pContainer->parse();

    GET_KEY(m_pContainer, 1, this->username, GKSTRING);
    GET_KEY(m_pContainer, 2, this->userID, GKINT);

    // thx cvolton: server autorenames nicknames with inappopriate words to GDUsr<random numbers>
    if (this->username.find("GDUsr") != std::string::npos) this->autoRenamed = true;
}
