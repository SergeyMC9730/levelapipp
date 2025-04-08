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

#include "ModuleEnabled.h"

#include "curl_backend.h"

#include <optional>
#include "LevelRangeList.h"

// search recent levels
#define GDSEARCH_RECENT "4"

namespace LevelAPI {
    class Account19;

    namespace DatabaseController {
        class Level;
    }

    namespace Backend {
        // current status of this gdserver
        enum GDServerStatus {
            // server works properly
            GSS_ONLINE = 0,
            // server is down
            GSS_OFFLINE = 1,
            // server's cloudflare banned ip that tried to make a request to this server
            GSS_PERMANENT_BAN = 2
        };

        class GDServerUploadResult {
        public:
            // is uploading was successful
            bool successful;
            // level id
            int id;
        };
        class GDServer : public ModuleEnabled {
        protected:
            // base url for this gdserver
            std::string m_sEndpointURL;

            // player username
            std::string m_sUsername;
            // player password
            std::string m_sPassword;
            // gjp encrypted password
            std::string m_sGJPPassword;
            // gjp2 encrypted password
            std::string m_sGJP2Password;

            // account for logged in user
            Account19 *m_pAccount;

            LevelRangeList _ranges;

            // run this server in debug mode
            bool m_bDebug;

            // get endpint for login action
            virtual std::string _getDownloadLevelEndpointName();
            // get endpoint for downloading level directly
            virtual std::string _getLevelListEndpointName();
            // get endpoint for downloading a list of levels
            virtual std::string _getLoginAccountEndpointName();

            // get common secret value used in the gd endpoints
            virtual std::string _getSecretValueStandard();
            // get extra secret value used in the gd endpoints
            // should be redone to be more specific
            virtual std::string _getSecretValueExtra();

            // get a list of cloudflare errors which tells user that their ip has been bannned
            virtual std::vector<std::string> _getCloudflareBans();

            // create curl connection for this request
            virtual CURLConnection *_setupCURL(std::optional<CurlProxy> proxy, std::string secret);
        public:
            enum SearchType {
                STQuery = 0,
                STMostDownloaded = 1,
                STMostLiked = 2,
                STTrending = 3,
                STRecent = 4,
                STUserLevels = 5,
                STFeatured = 6,
                STMagic = 7,
                STModSent = 8,
                STMapPack = 10,
                STAwarded = 11,
                STFollowed = 12,
                STFriends = 13,
                STWorldMostLiked = 15,
                STHallOfFlame = 16,
                STWorldFeatured = 17,
                STUnknownEmpty = 18,
                STUnknownMapPackPaged = 19,
                STDailyHistory = 21,
                STWeeklyHistory = 22,
                STLevelList = 25,
                STUnknownMapPackPaged100 = 26
            };
            enum SearchDifficulty {
                SDNa = -1,
                SDDemons = -2,
                SDEasy = 1,
                SDNormal = 2,
                SDHard = 3,
                SDHarder = 4,
                SDInsane = 5
            };
            enum SearchDemonDiff {
                SDDNone = 0,
                SDDEasy = 1,
                SDDMedium = 2,
                SDDHard = 3,
                SDDInsane = 4,
                SDDExtreme = 5
            };
            enum SearchLength {
                SLNone = -1,
                SLTiny = 0,
                SLShort = 1,
                SLMedium = 2,
                SLLong = 3,
                SLExtraLong = 4,
                SLPlatformer = 5
            };

            struct ExtendedParams {
                SearchType type = STQuery;
                SearchDifficulty diff = SDNa;
                SearchDemonDiff demon = SDDNone;
                SearchLength len = SLNone;
                std::string str = "";
                int page = 0;
                struct {
                    unsigned short featured : 1;
                    unsigned short original : 1;
                    unsigned short two_player : 1;
                    unsigned short coins : 1;
                    unsigned short epic : 1;
                    unsigned short legendary : 1;
                    unsigned short mythic : 1;
                    unsigned short no_star : 1;
                    unsigned short star : 1;
                } filter;
            };

            GDServer();
	        // create gdserver with a list of level ranges
            GDServer(std::vector<LevelRange> list);
            // create gdserver with a single level range
            GDServer(LevelRangeList list);

            ~GDServer();

	        // current status of this server
            GDServerStatus m_eStatus = GDServerStatus::GSS_ONLINE;
            // how much user should wait for a next request
            int _rateLimitLength = 0;

            // emab;e or disable debug mode
            virtual void setDebug(bool d);
            // get debug mode flag
            virtual bool getDebug();

	        // get level metadata by id
            virtual LevelAPI::DatabaseController::Level *getLevelMetaByID(int id, std::optional<CurlProxy> proxy = std::nullopt);
            // download level by id
            virtual LevelAPI::DatabaseController::Level *downloadLevel(int id, std::optional<CurlProxy> proxy = std::nullopt);
            // returns self
            virtual LevelAPI::DatabaseController::Level *resolveLevelData(LevelAPI::DatabaseController::Level *level, std::optional<CurlProxy> proxy = std::nullopt);
            // get a list of levels with type and standard parameters
	        virtual std::vector<LevelAPI::DatabaseController::Level *> getLevelsBySearchType(int type, std::optional<CurlProxy> proxy = std::nullopt);
            // implements basic search functionality
            // it doesn't allow searching requests that use gjp password
            virtual std::vector<LevelAPI::DatabaseController::Level *> getLevelsBySearchType(int type, std::string str, int page, std::optional<CurlProxy> proxy = std::nullopt);

            // implements full search functionality
            // it doesn't allow searching requests that require gjp password
            virtual std::vector<LevelAPI::DatabaseController::Level *> getLevels(ExtendedParams &params, std::optional<CurlProxy> proxy = std::nullopt);

            // get a list of levels based on the server response
            virtual std::vector<LevelAPI::DatabaseController::Level *> getLevelsFromResponse(std::string &response);

            // set credentials for this account
            virtual void setCredentials(std::string u, std::string p);
            // login into an acccount
            // NOT IMPLEMENTED
            virtual bool login(std::optional<CurlProxy> proxy = std::nullopt);

            // get max size of level page (in levels)
            virtual int getMaxLevelPageSize();
            // get max size of max pack page (in levels)
            virtual int getMaxMapPackPageSize();

	        // get gameversion for this server
            virtual int getGameVersion();

            // get server name
            virtual std::string getServerName();
            // get server identifier
            virtual std::string getServerIdentifier();

            // this function uses level ranges to detect in which version this level has been released
            virtual std::string determineGVFromID(int id);

            // try to upload a level
            // NOT IMPLEMENTED
            virtual GDServerUploadResult *uploadLevel(DatabaseController::Level *level, std::optional<CurlProxy> proxy = std::nullopt);

            // unload level array
            static void destroyLevelVector(std::vector<LevelAPI::DatabaseController::Level *> v);

            // returns true if curl or cloudflare returned error
            bool processCURLAnswer(CURLResult *res);

            static ExtendedParams createEmptyParams();
        };
    }
}
