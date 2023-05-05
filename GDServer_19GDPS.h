#pragma once

#include <string>
#include "GDServer_BoomlingsLike19.h"

namespace LevelAPI {
    namespace Backend {
        class GDServer_19GDPS : public GDServer_BoomlingsLike19 {
        public:
            GDServer_19GDPS(std::string endpoint);

            std::string getServerName() override;
        };
    }
}