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
#include <optional>

namespace LevelAPI {
    namespace Backend {
        class GDServer_BoomlingsLike19 : public GDServer {
        protected:
	        // get endpoint name for login action
            std::string _getLoginAccountEndpointName() override;
            // get endpoint for downloading level directly
            std::string _getDownloadLevelEndpointName() override;
            // get endpoint for downloading a list of levels
            std::string _getLevelListEndpointName() override;

            // get common secret value used in the gd endpoints
            std::string _getSecretValueStandard() override;
            // get extra secret value used in the gd endpoints.
	        // should be redone to be more specific
            std::string _getSecretValueExtra() override;

            virtual std::vector<CURLParameter *> _setupGJLevelsArgs(int type, std::string str, int page);
        public:
            GDServer_BoomlingsLike19(std::string endpoint);
            GDServer_BoomlingsLike19(std::string endpoint, std::vector<LevelRange> list);
            GDServer_BoomlingsLike19(std::string endpoint, LevelRangeList list);

	        // download level by id
            LevelAPI::DatabaseController::Level *downloadLevel(int id, std::optional<CurlProxy> proxy = std::nullopt) override;
            // implements basic search functionality.
            // it doesn't allow searching requests that use gjp
            std::vector<LevelAPI::DatabaseController::Level *> getLevelsBySearchType(int type, std::string str, int page, std::optional<CurlProxy> proxy = std::nullopt) override;
            // returns self
            LevelAPI::DatabaseController::Level *resolveLevelData(LevelAPI::DatabaseController::Level *level, std::optional<CurlProxy> proxy = std::nullopt) override;

            // login to an account.
            // NOT IMPLEMENTED
            bool login(std::optional<CurlProxy> proxy = std::nullopt) override;

            int getMaxLevelPageSize() override;
            int getMaxMapPackPageSize() override;

	        // try to upload a level.
	        // NOT IMPLEMENTED
            GDServerUploadResult *uploadLevel(DatabaseController::Level *level, std::optional<CurlProxy> proxy = std::nullopt) override;

            std::vector<LevelAPI::DatabaseController::Level *> getLevelsFromResponse(std::string &response) override;
            std::vector<LevelAPI::DatabaseController::Level *> getLevels(ExtendedParams &params, std::optional<CurlProxy> proxy = std::nullopt) override;

            int getGameVersion() override;
            std::string getServerName() override;

            std::string getServerIdentifier() override;
        };
    }
}
