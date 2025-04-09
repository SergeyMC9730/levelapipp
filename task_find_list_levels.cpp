/**
 *  LevelAPI - Geometry Dash level cacher with search functionality and more.
    Copyright (C) 2025  Sergei Baigerov

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

#include "GDServer.h"
#include "curl_backend.h"
#include "tasks.h"
#include <iostream>
#include <thread>
#include "GDServer_Boomlings22.h"
#include "Level.h"
#include <ostream>
#include <fstream>

struct task_ext_map {
    int levels;
    std::string naming;
};

void LevelAPI::Tasks::findListLevels() {
    std::cout << "TASK: Using GD server in 2.2 mode to obtain levels" << std::endl;

    auto boom = new LevelAPI::Backend::GDServer_Boomlings22("https://www.boomlings.com/database");
    LevelAPI::Backend::CurlProxy proxy = "socks5://127.0.0.1:9999";

    boom->setDebug(false);

    std::vector<LevelAPI::DatabaseController::Level *> lvls = {};

    srand(time(0));
    nlohmann::json j;

    std::map<Backend::GDServer::SearchDifficulty, task_ext_map> diffs = {
        {Backend::GDServer::SDEasy, {79, "easy"}},
        {Backend::GDServer::SDNormal, {206, "normal"}},
        {Backend::GDServer::SDHard, {1245, "hard"}},
        {Backend::GDServer::SDHarder, {1613, "harder"}},
        {Backend::GDServer::SDInsane, {700, "insane"}}
    };
    std::map<Backend::GDServer::SearchDemonDiff, task_ext_map> ddiffs = {
        {Backend::GDServer::SDDEasy, {234, "easy"}},
        {Backend::GDServer::SDDMedium, {245, "medium"}},
        {Backend::GDServer::SDDHard, {163, "hard"}},
        {Backend::GDServer::SDDInsane, {124, "insane"}},
        {Backend::GDServer::SDDExtreme, {124, "extreme"}}
    };

    for (auto [k, v] : diffs) {
        printf("[TASK] searching with difficulty %d\n", (int)k);

        std::string kn = "levels_" + v.naming;
        j[kn] = nlohmann::json::array();

        for (int i = 0; i < 20; i++) {
            std::this_thread::sleep_for(std::chrono::seconds(2 + boom->_rateLimitLength));

            printf("[TASK] searching page %d (%ld levels stored)\n", i, lvls.size());

            auto f = boom->createEmptyParams();
            f.filter.star = 1;
            f.diff = k;
            f.page = rand() % v.levels;
            f.len = Backend::GDServer::SLNone;
            f.type = Backend::GDServer::STMostDownloaded;
            f.str.clear();

            auto levels = boom->getLevels(f, proxy);
            if (!levels.empty()) {
                lvls.insert(lvls.end(), levels.begin(), levels.end());
            }
        }

        for (auto level : lvls) {
            level->generateJSON();
            j[kn].push_back(level->_jsonObject);
        }

        printf("[TASK] %ld LEVELS\n", lvls.size());

        boom->destroyLevelVector(lvls);
        lvls.clear();
    }

    for (auto [k, v] : ddiffs) {
        printf("[TASK] searching with demon difficulty %d\n", (int)k);

        std::string kn = "levels_d_" + v.naming;
        j[kn] = nlohmann::json::array();

        for (int i = 0; i < 20; i++) {
            std::this_thread::sleep_for(std::chrono::seconds(2 + boom->_rateLimitLength));

            printf("[TEST] searching page %d (%ld levels stored)\n", i, lvls.size());

            auto f = boom->createEmptyParams();
            f.filter.star = 1;
            f.diff = Backend::GDServer::SDDemons;
            f.demon = k;
            f.page = rand() % v.levels;
            f.len = Backend::GDServer::SLNone;
            f.type = Backend::GDServer::STMostDownloaded;
            f.str.clear();

            auto levels = boom->getLevels(f, proxy);
            if (!levels.empty()) {
                lvls.insert(lvls.end(), levels.begin(), levels.end());
            }
        }

        for (auto level : lvls) {
            j[kn].push_back({{"name", level->m_sLevelName}, {"id", level->m_nLevelID}});
        }

        printf("[TASK] %ld DEMON LEVELS\n", lvls.size());

        boom->destroyLevelVector(lvls);
        lvls.clear();
    }

    std::ofstream o("list.json");
    o << std::setw(4) << j << std::endl;
    o.close();

    delete boom;
    boom = nullptr;

    std::cout << "2.2 fetch has been completed" << std::endl;

    return;
}
