#pragma once

#include "GDServer_BasementLike21.h"

namespace LevelAPI {
    namespace Backend {
        class GDServer_BasementGDPS : public GDServer_BasementLike21 {
        public:
            GDServer_BasementGDPS(std::string endpoint);

            std::string getServerName() override;
            std::string getServerIdentifier() override;
        };
    }
}