/**
 *  LevelAPI - Geometry Dash level cacher with search functionality and more.
    Copyright (C) 2023  Sergei Baigerov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include "gmd2pp/GJGameLevel.h"

#include "json/single_include/nlohmann/json.hpp"

#include <dpp/dpp.h>

#include "LevelRelease.h"

#include "Time.h"

#include "DatabaseCell.h"

namespace LevelAPI {
    namespace DatabaseController {
      	// how level card should be appeared
        enum LevelAppearanceEvent {
            // render as the recently published level
            E_RECENT = 0,
            // render as the recentely registered level
            E_REGISTERED, 
	    // render as the recentely rated level
	    E_RATE,
            // render as the standard level
            E_INFORMATION
        };

        class Level : public LevelAPI::Imports::GeometryJump::GJGameLevel, public DatabaseCell {
        private:
            // how level can be downloaded inside this node
            std::string getDownloadLinks(bool embed);
        public:
            // game version for this level
            LevelRelease *m_uRelease;
            // level data path
            std::string m_sLevelPath = "";
            // registration timestamp
            std::string m_sCreatedTimestamp;
            // registration timestamp
            uint64_t m_nAppereanceTimestamp;

            // linked database node for this level
            std::string m_sLinkedNode = "";

            // full server response for this level
            std::string m_sRawData = "";

            bool m_bHasLevelString = false;
            bool m_bHasRawData = false;

            void setupJSON() override;
            Level(std::string linkedNode);
            void recover() override;
            void save(bool onlyLevelString = false);

            // get legacy timestamp for this level
            Frontend::Time *getTimeLegacy();

            // render this level as an discord embed
            dpp::embed getAsEmbed(LevelAppearanceEvent e);
            // returns filename without its path
            std::string generateDifficultyImage(std::string folder_prefix);

            ~Level();
        };
    }
}
