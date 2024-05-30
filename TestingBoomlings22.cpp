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

#include "TestingBoomlings22.h"
#include "GDServer_Boomlings22.h"
#include "Level.h"

#include <iostream>
#include <thread>

using namespace LevelAPI;

void Tests::test_boomlings_ver22() {
    std::cout << "Testing official GD server in 2.2 mode" << std::endl;

    auto boom = new LevelAPI::Backend::GDServer_Boomlings22("https://www.boomlings.com/database");

    boom->setDebug(false);

    std::cout << "New feature: download levels in batch. (4 levels)" << std::endl;

    auto levels = boom->fetchListOfLevels({128, 91682434, 91698864, 91707733}, 0);

    int i = 0;
    while(i < levels.size()) {
        auto level = levels[i];

        std::cout << " - Level \"" << level->m_sLevelName << "\" (" << std::to_string(level->m_nLevelID) << ")" << std::endl;
        
        delete level;
        levels[i] = nullptr;
        level = nullptr;

        i++;
    }

    delete boom;

    boom = nullptr;

    std::cout << "2.2 test has been completed" << std::endl;
    
    return;
}