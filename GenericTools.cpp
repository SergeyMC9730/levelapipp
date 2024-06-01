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

#include "GenericTools.hpp"

using namespace LevelAPI;

std::vector<std::string> GenericTools::getArguments(int argc, char *argv[]) {
    if (argc == 1) return {};

    int i = 1;
    std::vector<std::string> arguments = {};

    while(i < argc) {
        arguments.push_back(argv[i]);
        i++;
    }

    return arguments;
}