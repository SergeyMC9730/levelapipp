#pragma once

#include "GDServer_BoomlingsLike19.h"

namespace LevelAPI {
    namespace Backend {
        class GDServer_BoomlingsLike21 : public GDServer_BoomlingsLike19 {
        protected:
            std::string _getDownloadLevelEndpointName() override;
            std::string _getLevelListEndpointName() override;
        public:
            GDServer_BoomlingsLike21(std::string endpoint);

            int getGameVersion() override;
            std::string getServerName() override;

            std::string getServerIdentifier() override;
        };
    }
}