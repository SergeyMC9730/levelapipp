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
        enum GDPSActionType {
            GATRate = 0,
            GATFeature, GATEpic, GATGodlike, GATLegendary,
            GATCoins
        };

        class GDPSActionLog {
        public:
            std::string m_sModerator;
            GDPSActionType m_eType;
            std::string m_sValue1;
            std::string m_sValue2;
            int m_nLevelID;
            std::string m_sTimestamp;
        };
    }
}