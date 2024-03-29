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

#include <iostream>
#include <string>
#include <filesystem>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        std::cout << "Error: no folder selected" << std::endl;
        return -1;
    }

    std::string parameter = "";

    if (argc == 3) {
        parameter = argv[2];
    }

    if (parameter == "--help") {
        std::cout << "--with-names - show names" << std::endl;
        return 0;
    }

    std::string folder = argv[1];

    int files = 0;

    std::cout << "Folder: " + folder << std::endl;

    try {
        if (parameter == "--with-names") {
            for (const auto & entry : std::filesystem::directory_iterator(folder)) {
                std::string path = entry.path();
                std::string filename = path.substr(path.find_last_of("/\\") + 1);
                std::cout << "Name for " << ++files << ": " << filename << std::endl;
            }
        } else {
            for (const auto & entry : std::filesystem::directory_iterator(folder)) files++;
        }
    } catch (std::filesystem::filesystem_error) {
        std::cout << "Error: no such directory" << std::endl;
        return -1;
    }

    std::cout << "Files: " << files << std::endl;

    return 0;
}