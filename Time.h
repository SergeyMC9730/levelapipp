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
#include <vector>
#include <cstdint>

namespace LevelAPI {
    namespace Frontend {
        class Time {
        protected:
            std::string time_hms;
            int time_h;
            int time_m;
            int time_s;
        public:
            Time(uint64_t unixTimestamp = 0);
        public:
            uint64_t unixTime;

            static Time *create();
            void fromTimeString(std::string str);

            std::string getAsString();
            uint64_t getAsInt64();
        };
    }
}