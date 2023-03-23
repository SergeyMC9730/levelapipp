#pragma once

#include "curl_backend.h"

#include <string>
#include <vector>

#include "gmd2pp/level-converter/GJGameLevel.h"
#include "lapi_database.h"

#define GDSEARCH_RECENT "4"

namespace LevelAPI {
    namespace Backend {
        class GDServer {
        protected:
            std::string *m_sEndpointURL;
            CURLConnection *m_pLinkedCURL;
        public:
            GDServer();
            ~GDServer();

            virtual GJGameLevel *getLevelMetaByID(int id);
            // returns self
            virtual LevelAPI::DatabaseController::Level *resolveLevelData(LevelAPI::DatabaseController::Level *level);
            virtual std::vector<LevelAPI::DatabaseController::Level *> getLevelsBySearchType(int type);
        };
    }
}