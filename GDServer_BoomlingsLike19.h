#pragma once

#include "GDServer.h"

namespace LevelAPI {
    namespace Backend {
        class GDServer_BoomlingsLike19 : public GDServer {
        protected:
            std::string _getLoginAccountEndpointName() override;
            std::string _getDownloadLevelEndpointName() override;
            std::string _getLevelListEndpointName() override;

            std::string _getSecretValueStandard() override;
            std::string _getSecretValueExtra() override;
        public:
            GDServer_BoomlingsLike19(std::string endpoint);

            LevelAPI::DatabaseController::Level *getLevelMetaByID(int id, bool resolveAccountInfo, std::optional<CurlProxy> proxy = std::nullopt) override;
            std::vector<LevelAPI::DatabaseController::Level *> getLevelsBySearchType(int type, std::string str, int page) override;
            // returns self
            LevelAPI::DatabaseController::Level *resolveLevelData(LevelAPI::DatabaseController::Level *level, std::optional<CurlProxy> proxy = std::nullopt) override;

            bool login() override;

            GDServerUploadResult *uploadLevel(DatabaseController::Level *level) override;

            int getGameVersion() override;
            std::string getServerName() override;

            std::string getServerIdentifier() override;
            
        };
    }
}