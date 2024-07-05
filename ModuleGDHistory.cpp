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

#include "ModuleGDHistory.h"
#include "Level.h"
#include <fmt/format.h>

#include "curl_backend.h"

using namespace LevelAPI::DatabaseController;
using namespace LevelAPI::Backend;

std::string ModuleGDHistory::getAPIPath() {
    return "https://history.geometrydash.eu/api";
}

Level *ModuleGDHistory::downloadArchivedLevel(int id) {
    Level *level = nullptr;

    auto url = fmt::format("{}/v1/level/{}/brief", 
        getAPIPath(),
        id
    );

    auto con = new CURLConnection();

    auto result = con->access_page(url.c_str());
    
    if (result->http_status == 200) {
        std::string data = result->data;

        free((void *)result->data);
        result->data = nullptr;

        nlohmann::json j = nlohmann::json::parse(data);

        if (j.contains("success") && j["success"] == false) {
            delete con;
            con = nullptr;
            
            delete result;
            result = nullptr;

            return level;
        }

        level = new Level("");

        #define LOAD_DATA(str1, str2) if (j.contains(str2) && !j[str2].is_null()) level->_jsonObject[str1] = j[str2];

        level->_jsonObject["levelID"] = id;
        LOAD_DATA("version", "cache_available_versions")
        LOAD_DATA("playerID", "cache_user_id");
        LOAD_DATA("downloads", "cache_downloads");
        LOAD_DATA("likes", "cache_likes");
        LOAD_DATA("length", "cache_length");
        LOAD_DATA("difficulty_denominator", "cache_rating");
        LOAD_DATA("difficulty_numerator", "cache_rating_sum");
        LOAD_DATA("stars", "cache_stars");
        LOAD_DATA("copiedFrom", "cache_original");
        LOAD_DATA("dailyNumber", "cache_daily_id");
        LOAD_DATA("isEpic", "cache_epic");
        LOAD_DATA("demonDifficulty", "cache_demon_type");
        LOAD_DATA("appereanceTimestamp", "cache_submitted_timestamp");
        LOAD_DATA("isAuto", "cache_auto");
        LOAD_DATA("isDemon", "cache_demon");
        LOAD_DATA("is2P", "cache_two_player");
        LOAD_DATA("levelName", "cache_level_name");
        LOAD_DATA("username", "cache_username");

        level->recover();
    }

    delete result;
    result = nullptr;

    delete con;
    con = nullptr;

    return level;
}