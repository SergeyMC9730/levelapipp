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

#include <filesystem>
#include <string>
#include <vector>
#include <iostream>

#include "GenericTools.hpp"

using namespace LevelAPI::GenericTools;

void process_level(std::string path, std::string filename) {
    std::string new_filename = std::string(filename.c_str() + 6);
    std::string res = "";

    for (char num : new_filename) {
        char v[2] = {num, 0};

        res += std::string(v) + "/";
    }

    if (res.length() >= 1) res.pop_back();

    std::string new_path = path + "/" + res;
    std::string old_path = path + "/" + filename;

    // std::cout << res << std::endl;

    bool _r1 = std::filesystem::create_directories(new_path);;

    try {
        std::filesystem::copy(old_path, new_path);
    } catch (std::filesystem::filesystem_error e) {
        std::cout << "warn: " << e.what() << std::endl;
    }
}

void print_process_info(int a, int b) {
    std::cout << "level migration done: " << a << " levels were affected; " << b << " levels were ignored (cannot be migrated); " << a + b << " in total" << std::endl;
}

void begin_mirgration(std::string path) {
    int levels_affected = 0;
    int levels_ignored = 0;

    if (!std::filesystem::exists(path)) {
        print_process_info(0, 0);

        return;
    }

    for (const auto & entry : std::filesystem::directory_iterator(path)) {
        std::string _path = entry.path();
        std::string filename = _path.substr(_path.find_last_of("/\\") + 1);

        if (filename.rfind("Level_", 0) == 0) {
            process_level(path, filename);

            levels_affected++;
        } else {
            levels_ignored++;
        }
    }

    print_process_info(levels_affected, levels_ignored);
}

int main(int argc, char **argv) {
    auto args = getArguments(argc, argv);

    if (args.size() == 0) {
        std::cout << "error: provide levels folder path" << std::endl;

        return 1;
    }

    std::string path = args[0];

    begin_mirgration(path);
    
    return 0;
}