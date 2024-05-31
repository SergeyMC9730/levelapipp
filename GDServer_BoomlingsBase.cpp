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

#include "GDServer_BoomlingsBase.h"
#include "LevelRange.h"
#include "ModuleGDHistory.h"
#include "Level.h"
#include "LevelRelease.h"

using namespace LevelAPI::Backend;

LevelAPI::LevelRangeList GDServer_BoomlingsBase::getRanges() {
    std::vector<LevelAPI::LevelRange> m_vRanges;

    m_vRanges.push_back({91, 1941, "1.0"});
    m_vRanges.push_back({1942, 10043, "1.1"});
    m_vRanges.push_back({10044, 63415, "1.2"});
    m_vRanges.push_back({63416, 121068, "1.3"});
    m_vRanges.push_back({121069, 184425, "1.4"});
    m_vRanges.push_back({184426, 420780, "1.5"});
    m_vRanges.push_back({420781, 827308, "1.6"});
    m_vRanges.push_back({827309, 1627362, "1.7"});
    m_vRanges.push_back({1627363, 2810918, "1.8"});
    m_vRanges.push_back({2810919, 11020426, "1.9"});
    m_vRanges.push_back({11020427, 28356225, "2.0"});
    m_vRanges.push_back({28356226, 99900002, "2.1"});
    m_vRanges.push_back({99000002, 100800002, "2.2"});

    return m_vRanges;
}

GDServer_BoomlingsBase::GDServer_BoomlingsBase() {
    ModuleGDHistory::mod_init();
}

LevelAPI::DatabaseController::Level *GDServer_BoomlingsBase::downloadArchivedLevel(int id) {
    auto level = ModuleGDHistory::downloadArchivedLevel(id);

    auto ranges = getRanges();

    if (level) {
        level->m_uRelease->m_fActualVersion = ranges[id];
    }

    return level;
};