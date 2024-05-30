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

#include <string>

namespace LevelAPI {
    namespace Backend {
	// type of action happened on a GDPS
        enum GDPSActionType {
	    // level has been rated
            GATRate = 0,
            // level has been featured
            GATFeature,
            // level has been rated to epic
            GATEpic,
            // level has been rated to godlike
            GATGodlike,
            // level has been rated to legendary
            GATLegendary,
            // level coins were been approved
            GATCoins
        };

        class GDPSActionLog {
        public:
            // which moderator made this action
            std::string m_sModerator;
            // which action it is
            GDPSActionType m_eType;
            // custom value 1
            std::string m_sValue1;
            // custom value 2
            std::string m_sValue2;
            // which level has been affected
            int m_nLevelID;
            // when this action happened
            std::string m_sTimestamp;
        };
    }
}
