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
#include <map>

extern std::string translation_language;

#include "fmt/include/fmt/core.h"

namespace LevelAPI {
    namespace Frontend {
        namespace Translation {
            typedef struct translation_unit_t {
                std::string en;
                std::string ru;
            } translation_unit_t;

            extern std::map<std::string, translation_unit_t> translation_keymap;
        
            template <typename... Args>
            inline std::string getByKey(std::string key, Args... args) {
                std::string str = "";
                if (translation_keymap.find(key) == translation_keymap.end()) {
                    str = key;
                } else {
                    auto multistr = translation_keymap[key];
                    
                    if (translation_language == "en") str = multistr.en;
                    if (translation_language == "ru") str = multistr.ru;
                }
                fmt::format_string<Args...> data = str;
                return fmt::format(data, std::forward<Args>(args)...);
            }

            void init();
        }
    }
}