#pragma once

#include "GDServer.h"
#include "lapi_database.h"

namespace LevelAPI {
    namespace Backend {
        class GDServer_BasementLike21 : public GDServer {
        public:
            GDServer_BasementLike21(std::string endpoint);

            LevelAPI::DatabaseController::Level *getLevelMetaByID(int id, bool resolveAccountInfo) override;
            std::vector<LevelAPI::DatabaseController::Level *> getLevelsBySearchType(int type, std::string str, int page) override;
            // returns self
            LevelAPI::DatabaseController::Level *resolveLevelData(LevelAPI::DatabaseController::Level *level) override;
        
            GDServerUploadResult *uploadLevel(DatabaseController::Level *level) override;

            bool login() override;

            int getGameVersion() override;
            std::string getServerName() override;
            std::string getServerIdentifier() override;
        };
    }
}