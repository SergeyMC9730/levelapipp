#pragma once

#include "gmd2pp/GJGameLevel.h"

#include "json/single_include/nlohmann/json.hpp"

#include <dpp/dpp.h>

#include "LevelRelease.h"

#include "Time.h"

#include "DatabaseCell.h"

namespace LevelAPI {
    namespace DatabaseController {
        enum LevelAppearanceEvent {
            E_RECENT = 0,
            E_REGISTERED, E_RATE,
            E_INFORMATION
        };

        class Level : public GJGameLevel, public DatabaseCell {
        private:
            std::string getDownloadLinks(bool embed);
        public:
            LevelRelease *m_uRelease;
            std::string m_sLevelPath = "";
            std::string m_sCreatedTimestamp;
            uint64_t m_nAppereanceTimestamp;

            std::string m_sLinkedNode = "";

            std::string m_sRawData = "";

            int m_nRetryAfter = 0;

            bool m_bHasLevelString = false;
            bool m_bHasRawData = false;

            void setupJSON() override;
            Level(std::string linkedNode);
            void recover() override;
            void save(bool onlyLevelString = false);

            Frontend::Time *getTimeLegacy();

            dpp::embed getAsEmbed(LevelAppearanceEvent e);
            // returns filename without its path
            std::string generateDifficultyImage(std::string folder_prefix);

            ~Level();
        };
    }
}