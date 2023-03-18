#pragma once

#include "curl_backend.h"

#include <string>
#include <vector>

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
            virtual std::vector<LevelAPI::DatabaseController::Level *> getLevelsBySearch(const char *type);
        };
    }
}