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
        };
    }
}
