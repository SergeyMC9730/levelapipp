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

#include "GDServer_BoomlingsLike21.h"
#include <optional>

namespace LevelAPI {
    namespace Backend {
        class GDServer_BoomlingsLike22 : public GDServer_BoomlingsLike21 {
        public:
            GDServer_BoomlingsLike22(std::string endpoint);
            GDServer_BoomlingsLike22(std::string endpoint, std::vector<LevelRange> list);
            GDServer_BoomlingsLike22(std::string endpoint, LevelRangeList list);

	    // get metadata for a list of levels
            virtual std::vector<LevelAPI::DatabaseController::Level *> fetchListOfLevels(std::vector<int> levels, int page, std::optional<CurlProxy> proxy = std::nullopt);

	    // get list of latest reported levels.
	    // UNTESTED
            virtual std::vector<LevelAPI::DatabaseController::Level *> getReportedLevels(std::optional<CurlProxy> proxy = std::nullopt);

            int getGameVersion() override;
            std::string getServerName() override;

            std::string getServerIdentifier() override;
        };
    }
}
