#pragma once

#include "GDServer.h"
#include "lapi_database.h"

namespace LevelAPI {
    namespace Backend {
        class GDServer_BoomlingsLike19 : public GDServer {
        public:
            GDServer_BoomlingsLike19(std::string *endpoint);

            LevelAPI::DatabaseController::Level *getLevelMetaByID(int id, bool resolveAccountInfo) override;
            std::vector<LevelAPI::DatabaseController::Level *> getLevelsBySearchType(int type) override;
            // returns self
            LevelAPI::DatabaseController::Level *resolveLevelData(LevelAPI::DatabaseController::Level *level) override;

            int getGameVersion() override;
        };
    }
}