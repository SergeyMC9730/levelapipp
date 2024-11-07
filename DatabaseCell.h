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

#include "json/single_include/nlohmann/json.hpp"

class SQLiteManager;

namespace LevelAPI {
    namespace DatabaseController {
        class DatabaseCell {
        public:
            // sqlite instance if we want to use it
            SQLiteManager *_sqliteObject = nullptr;
            // json data for saving and recovering the cell
            nlohmann::json _jsonObject = nlohmann::json::object();
        public:
            // destructor
            ~DatabaseCell();

            // save cell data into json
            virtual void save();
            // recover cell from json
            virtual void recover();

            // setup json data
            virtual void setupJSON();
            // create sqlite instance
            virtual void setupSQLite();

            operator nlohmann::json () const {
                return _jsonObject;
            }
        };
    }
}
