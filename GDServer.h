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

#include "Account19.h"
#include "CURLParameter.h"
#include "curl_backend.h"

#include <string>
#include <vector>
#include <optional>

#include "CurlProxy.h"

#include "Level.h"
#include "LevelRangeList.h"

#define GDSEARCH_RECENT "4"

enum GDServerStatus {
    GSS_ONLINE = 0,
    GSS_OFFLINE = 1,
    GSS_PERMANENT_BAN = 2
};

namespace LevelAPI {
    namespace Backend {
        class GDServerUploadResult {
        public:
            bool successful;
            int id;
        };
        class GDServer {
        protected:
            std::string m_sEndpointURL;
            
            std::string m_sUsername;
            std::string m_sPassword;
            std::string m_sGJPPassword;

            Account19 *m_pAccount;

            LevelRangeList _ranges;

	        bool m_bDebug;

            virtual std::string _getDownloadLevelEndpointName();
            virtual std::string _getLevelListEndpointName();
            virtual std::string _getLoginAccountEndpointName();

            virtual std::string _getSecretValueStandard();
            virtual std::string _getSecretValueExtra();
        public:
            GDServer();
            GDServer(std::vector<LevelRange> list);
            GDServer(LevelRangeList list);

            ~GDServer();

            GDServerStatus m_eStatus;

            virtual void setDebug(bool d);
            virtual bool getDebug();

            virtual LevelAPI::DatabaseController::Level *getLevelMetaByID(int id, bool resolveAccountInfo, std::optional<CurlProxy> proxy = std::nullopt);
            // returns self
            virtual LevelAPI::DatabaseController::Level *resolveLevelData(LevelAPI::DatabaseController::Level *level, std::optional<CurlProxy> proxy = std::nullopt);
            virtual std::vector<LevelAPI::DatabaseController::Level *> getLevelsBySearchType(int type);
            virtual std::vector<LevelAPI::DatabaseController::Level *> getLevelsBySearchType(int type, std::string str, int page);
        
            virtual void setCredentials(std::string u, std::string p);
            virtual bool login();

            virtual int getMaxLevelPageSize();
            virtual int getMaxMapPackPageSize();

            virtual int getGameVersion();
            virtual std::string getServerName();
            virtual std::string getServerIdentifier();

            std::string determineGVFromID(int id);

            virtual GDServerUploadResult *uploadLevel(DatabaseController::Level *level);
        };
    }
}
