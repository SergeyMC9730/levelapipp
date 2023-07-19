#pragma once

#include "GDServer.h"
#include "GDServer_BoomlingsLike21.h"
#include "Level.h"
#include <vector>

namespace LevelAPI {
    namespace Backend {
        class GDServer_BoomlingsLike22 : public GDServer_BoomlingsLike21 {
        public:
            GDServer_BoomlingsLike22(std::string endpoint);

            virtual std::vector<LevelAPI::DatabaseController::Level *> fetchListOfLevels(std::vector<int> levels);

            virtual std::vector<LevelAPI::DatabaseController::Level *> getReportedLevels();

            int getGameVersion() override;
            std::string getServerName() override;

            std::string getServerIdentifier() override;
        };
    }
}