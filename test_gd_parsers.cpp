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

#include "lapi_database.h"
#include "tests.h"
#include "Level.h"

#include <iostream>
#include <thread>
#include <chrono>

#include "GDServer_BoomlingsLike21.h"

#include "Translation.h"

using namespace LevelAPI;
using namespace std::chrono_literals;
using namespace LevelAPI::Frontend;

bool Tests::testGDParsers() {
    std::cout << Translation::getByKey("lapi.gdpastertests.start");

    std::thread tr(Tests::testGDParsersThread);
    tr.join();

    return true;
}
void Tests::testGDParsersThread() {
    auto gd = new Backend::GDServer_BoomlingsLike21("https://www.boomlings.com/database");
    auto levels = gd->getLevelsBySearchType(4, "", 0);

    std::cout << Translation::getByKey("lapi.gdpastertests.test1result", levels.size());

    std::this_thread::sleep_for(600ms);

    auto somelevel = gd->resolveLevelData(levels[0]);
    somelevel->save();

    levels.clear();
    delete gd;
    gd = nullptr;

    std::cout << Translation::getByKey("lapi.gdpastertests.complete");

    return;
}
