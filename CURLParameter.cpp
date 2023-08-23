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

#include "curl_backend.h"
#include <string>

using namespace LevelAPI::Backend;

CURLParameter::CURLParameter() {
    key = "";
    value = "";
}
CURLParameter::CURLParameter(std::string k, std::string v) {
    key = k;
    value = v;
}
CURLParameter::CURLParameter(std::string k, int v) {
    key = k;
    value = std::to_string(v);
}