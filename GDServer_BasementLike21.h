#pragma once

#include "GDServer_BoomlingsLike21.h"
#include "lapi_database.h"

namespace LevelAPI {
    namespace Backend {
        class GDServer_BasementLike21 : public GDServer_BoomlingsLike21 {
        public:
            GDServer_BasementLike21(std::string endpoint);

            std::string getServerName() override;
            std::string getServerIdentifier() override;
        };
    }
}