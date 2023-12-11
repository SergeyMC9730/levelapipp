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

#include "tests.h"
#include "GDServer_BasementGDPS.h"

#include <iostream>

#include "fmt/include/fmt/core.h"

using namespace LevelAPI;
// using namespace LevelAPI::Frontend;

void Tests::testBasementFeatures() {
    std::cout << "-- Testing Basement GDPS Features --" << std::endl;
    std::cout << "-1 Testing Recent Rates 1-" << std::endl;

    int list_length = 10;

    auto server = new LevelAPI::Backend::GDServer_BasementGDPS("https://podvalgdsherov.fun/pgcore");

    auto levels = server->getRecentRatedLevels(list_length, std::nullopt);
    
    printf("-1 Found %zu levels 1-\n", levels.size());

    for (auto level : levels) {
        std::string l = fmt::format(" * Level {} by {}\n ** feature? {}\n ** epic? {}\n ** stars? {}",
            level->m_sLevelName, level->m_sUsername,
            level->m_nFeatureScore, level->m_nEpic, level->m_nStars
        );

        std::cout << l << std::endl;
    }

    server->destroyLevelVector(levels);

    delete server;

    return;
}