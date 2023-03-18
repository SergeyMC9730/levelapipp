#pragma once

#include "GDServer.h"

namespace LevelAPI {
    namespace Backend {
        class GDServer_BoomlingsLike21 : public GDServer {
        public:
            GDServer_BoomlingsLike21(std::string *endpoint);

            GJGameLevel *getLevelMetaByID(int id) override;
            std::vector<LevelAPI::DatabaseController::Level *> getLevelsBySearch(const char *type) override;
        };
    }
}