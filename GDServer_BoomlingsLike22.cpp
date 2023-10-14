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

#include "GDServer_BoomlingsLike22.h"
#include "LevelRangeList.h"
#include "Translation.h"

using namespace LevelAPI::Backend;

GDServer_BoomlingsLike22::GDServer_BoomlingsLike22(std::string endpoint) : GDServer_BoomlingsLike21(endpoint) {}
GDServer_BoomlingsLike22::GDServer_BoomlingsLike22(std::string endpoint, std::vector<LevelRange> list) : GDServer_BoomlingsLike21(endpoint, list) {}
GDServer_BoomlingsLike22::GDServer_BoomlingsLike22(std::string endpoint, LevelRangeList list) : GDServer_BoomlingsLike21(endpoint, list) {}

int GDServer_BoomlingsLike22::getGameVersion() {
    return 22;
}

std::vector<LevelAPI::DatabaseController::Level *> GDServer_BoomlingsLike22::fetchListOfLevels(std::vector<int> levels, int page, std::optional<CurlProxy> proxy) {
    if (levels.size() > 100) return {nullptr};

    std::string str = "";

    int size = levels.size();

    int i = 0;
    while(i < size) {
        str += std::to_string(levels[i]);

        if (i + 1 != size) {
            str += ",";
        }

        i++;
    }
    
    return getLevelsBySearchType(26, str, page, proxy);
}

std::string GDServer_BoomlingsLike22::getServerName() {
    return Frontend::Translation::getByKey("lapi.gdserver_boomlingslike22.name");
}

std::string GDServer_BoomlingsLike22::getServerIdentifier() {
    return "GDServer_BoomlingsLike22";
}

std::vector<LevelAPI::DatabaseController::Level *> GDServer_BoomlingsLike22::getReportedLevels(std::optional<CurlProxy> proxy) {
    return {}; // cannot test tho
}