#pragma once

#include "GDServer.h"
#include "GDServer_BoomlingsLike21.h"
#include "Level.h"
#include "lapi_database.h"
#include <vector>

namespace LevelAPI {
    namespace Backend {
        class GDServer_BoomlingsLike22 : public GDServer_BoomlingsLike21 {
        public:
            GDServer_BoomlingsLike22(std::string endpoint);

            // LevelAPI::DatabaseController::Level *getLevelMetaByID(int id, bool resolveAccountInfo) override;
            // std::vector<LevelAPI::DatabaseController::Level *> getLevelsBySearchType(int type, std::string str, int page) override;
            // // returns self
            // LevelAPI::DatabaseController::Level *resolveLevelData(LevelAPI::DatabaseController::Level *level) override;
        
            // GDServerUploadResult *uploadLevel(DatabaseController::Level *level) override;

            // bool login() override;

            virtual std::vector<LevelAPI::DatabaseController::Level *> fetchListOfLevels(std::vector<int> levels);

            virtual std::vector<LevelAPI::DatabaseController::Level *> getReportedLevels();

            int getGameVersion() override;
            std::string getServerName() override;

            std::string getServerIdentifier() override;
        };
    }
}