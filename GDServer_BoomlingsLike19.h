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

#include "GDServer.h"
#include "LevelRange.h"
#include "LevelRangeList.h"

namespace LevelAPI {
    namespace Backend {
        class GDServer_BoomlingsLike19 : public GDServer {
        protected:
            std::string _getLoginAccountEndpointName() override;
            std::string _getDownloadLevelEndpointName() override;
            std::string _getLevelListEndpointName() override;

            std::string _getSecretValueStandard() override;
            std::string _getSecretValueExtra() override;
        public:
            GDServer_BoomlingsLike19(std::string endpoint);
            GDServer_BoomlingsLike19(std::string endpoint, std::vector<LevelRange> list);
            GDServer_BoomlingsLike19(std::string endpoint, LevelRangeList list);

            LevelAPI::DatabaseController::Level *getLevelMetaByID(int id, bool resolveAccountInfo, std::optional<CurlProxy> proxy = std::nullopt) override;
            std::vector<LevelAPI::DatabaseController::Level *> getLevelsBySearchType(int type, std::string str, int page, std::optional<CurlProxy> proxy = std::nullopt) override;
            // returns self
            LevelAPI::DatabaseController::Level *resolveLevelData(LevelAPI::DatabaseController::Level *level, std::optional<CurlProxy> proxy = std::nullopt) override;

            bool login(std::optional<CurlProxy> proxy = std::nullopt) override;

            int getMaxLevelPageSize() override;
            int getMaxMapPackPageSize() override;

            GDServerUploadResult *uploadLevel(DatabaseController::Level *level, std::optional<CurlProxy> proxy = std::nullopt) override;

            int getGameVersion() override;
            std::string getServerName() override;

            std::string getServerIdentifier() override;
            
        };
    }
}