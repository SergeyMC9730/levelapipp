#pragma once

#include "gmd2pp/GJGameLevel.h"

#include "json/single_include/nlohmann/json.hpp"

#include <dpp/dpp.h>

#include "LevelRelease.h"

namespace LevelAPI {
    namespace DatabaseController {
        enum LevelAppearanceEvent {
            E_RECENT = 0,
            E_REGISTERED, E_RATE
        };

        class Level : public GJGameLevel {
        private:
            std::string getDownloadLinks(bool embed);
        public:
            nlohmann::json levelJson;
            LevelRelease *m_uRelease;
            std::string m_sLevelPath;
            std::string m_sCreatedTimestamp;

            std::string m_sLinkedNode;

            std::string m_sRawData;

            int m_nRetryAfter;

            bool m_bHasLevelString;
            bool m_bHasRawData;

            void setupJSON();
            Level();
            void restore();
            void save(bool onlyLevelString = false);

            dpp::embed getAsEmbed(LevelAppearanceEvent e);
            // returns filename without its path
            std::string generateDifficultyImage(std::string folder_prefix);

            ~Level();
        };
    }
}