#pragma once

#include "gmd2pp/GJGameLevel.h"

#include "json/single_include/nlohmann/json.hpp"

#include <dpp/dpp.h>

#include "LevelRelease.h"

namespace LevelAPI {
    namespace DatabaseController {
        class Level : public GJGameLevel {
        public:
            nlohmann::json levelJson;
            LevelRelease *m_uRelease;
            std::string m_sLevelPath;
            int m_nAccountID;
            std::string m_sUsername;

            std::string m_sCreatedTimestamp;

            std::string m_sLinkedNode;

            int m_nRetryAfter;
            bool m_bHasLevelString;

            void setupJSON();
            Level();
            void restore();
            void save(bool onlyLevelString = false);

            dpp::embed getAsEmbed();

            ~Level();
        };
    }
}