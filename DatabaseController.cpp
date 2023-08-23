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

#include "lapi_database.h"

#include <iostream>
#include <sys/stat.h>
#include <vector>
#include <fstream>

#include "json/single_include/nlohmann/json.hpp"

using namespace std;

LevelAPI::DatabaseController::Database *LevelAPI::DatabaseController::database;

void LevelAPI::DatabaseController::setup() {
    #define db LevelAPI::DatabaseController::database

    std::string prefix = "database";

    std::vector<std::string> fpaths = {
        prefix, 
        prefix + "/configuration", 
        prefix + "/nodes"
    };

    int i = 0;
    while(i < fpaths.size()) {
        mkdir(fpaths[i].c_str(), 0777);
        i++;
    }
    
    db = new Database(fpaths[0]);    

    return;
}