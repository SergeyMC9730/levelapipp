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

#pragma once

#include <string>

#include "RobTopStringContainer.hpp"

namespace LevelAPI {
    class Account10 {
    protected:
        // KV array
        RobTopStringContainer *m_pContainer;

        // adds parseres for our KV container
        virtual void addParsers();
    public:
        /*
            If username starts with GDUsrXXXX,
            where XXXX is a number,  this user
            was actually renamed automatically
        */
        bool autoRenamed;
        // username
        std::string username;
        // user ID
        int userID;

        // constructor
        Account10();
        // destructor
        ~Account10();

        // parse userdata from string
        virtual void parseFromString(std::string server_response);
    };
}