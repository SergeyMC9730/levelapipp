#pragma once

#include "Account19.h"
#include "curl_backend.h"

#include <string>
#include <vector>

#include "lapi_database.h"


#include "Tools.h"

#define GDSEARCH_RECENT "4"

namespace LevelAPI {
    namespace Backend {
        class GDServerUploadResult {
        public:
            bool successful;
            int id;
        };
        class GDServer {
        protected:
            std::string *m_sEndpointURL;
            
            std::string *m_sUsername;
            std::string *m_sPassword;

            Account19 *m_pAccount;

            std::vector<Tools::LevelRange *> m_vRanges;

	        bool m_bDebug;
        public:
            GDServer();
            ~GDServer();

            virtual void setDebug(bool d);
            virtual bool getDebug();

            virtual LevelAPI::DatabaseController::Level *getLevelMetaByID(int id, bool resolveAccountInfo);
            // returns self
            virtual LevelAPI::DatabaseController::Level *resolveLevelData(LevelAPI::DatabaseController::Level *level);
            virtual std::vector<LevelAPI::DatabaseController::Level *> getLevelsBySearchType(int type);
        
            virtual void setCredintials(std::string u, std::string p);
            virtual bool login();

            virtual int getGameVersion();

            virtual std::string determineGVFromID(int id);

            virtual GDServerUploadResult *uploadLevel(DatabaseController::Level *level);
        };
    }
}
