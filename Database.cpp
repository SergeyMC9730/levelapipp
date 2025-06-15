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

#include "TelegramInstance.hpp"
#ifdef _DPP_ENABLED_
#include "DiscordInstance.h"
#include <dpp/dpp.h>
#endif
#include "lapi_database.h"
#include "json/single_include/nlohmann/json.hpp"
#include "DatabaseControllerThreads.h"
#include <filesystem>
#include <fstream>
#include <vector>
#include <iostream>
#include <chrono>
#include <malloc.h>
#include <sys/stat.h>
#include <thread>
#include <filesystem>

#include "Translation.h"
#include "Tools.h"

using namespace LevelAPI::DatabaseController;

bool Database::exists(){
    struct stat buffer;
    std::string p = databasePath + "/info.json";
    return std::filesystem::exists(p);
}

Database::Database(std::vector<Node *> nodes) {
    m_nNodeSize = nodes.size();
    m_vNodes = nodes;

    setupJSON();

    _jsonObject["nodes"] = nlohmann::json::array();
    _jsonObject["nodeSize"] = m_nNodeSize;

    save();
}
Database::Database(Node *node) {
    m_vNodes.push_back(node);
    m_nNodeSize = m_vNodes.size();

    setupJSON();

    _jsonObject["nodes"] = nlohmann::json::array();
    _jsonObject["nodeSize"] = m_nNodeSize;

    save();
}
Database::Database(std::string path) {
    databasePath = path;

    std::string p1 = path + "/info.json";
    std::string p2 = path + "/info_backup.json";

    m_nNodeSize = m_vNodes.size();

    if(!std::filesystem::exists(p1)) {
        setupJSON();

        _jsonObject["nodes"] = nlohmann::json::array();
        _jsonObject["nodeSize"] = m_nNodeSize;

        save();

        return;
    }

    std::ifstream i(p1);
    try {
        _jsonObject = nlohmann::json::parse(i);
    } catch (nlohmann::detail::parse_error e) {
        if (std::filesystem::exists(p2)) {
            std::cout << "Loading from the backup file" << std::endl;

            m_bLoadedFromBackup = true;

            std::ifstream i2(p2);
            _jsonObject = nlohmann::json::parse(i2);

            i2.close();
        } else {
            std::cout << "Backup file is not available" << std::endl;
        }
    }

    i.close();

    GET_JSON_VALUE(_jsonObject, "nodeSize", m_nNodeSize, int);
    int ii = 0;
    while(ii < _jsonObject["nodes"].size()) {
        auto json_object = _jsonObject["nodes"].at(ii);

        auto createdNode = new Node(json_object["internalName"].get<std::string>());

        m_vNodes.push_back(createdNode);

        createdNode->_jsonObject = json_object;
        createdNode->recover();

        ii++;
    }

    recalculate();

#ifdef _DPP_ENABLED_
    if (_jsonObject.contains("botToken")) {
        discord.m_bEnableBot = true;
        GET_JSON_VALUE(_jsonObject, "botToken", discord.m_sBotToken, std::string);
    }
    GET_JSON_VALUE(_jsonObject, "registeredCID", discord.m_sRegisteredCID, std::string);
    GET_JSON_VALUE(_jsonObject, "registeredCID2", discord.m_sRegisteredCID2, std::string);

    GET_JSON_VALUE(_jsonObject, "language", translation_language, std::string);

    if(discord.m_bEnableBot && !discord.m_sBotToken.empty()) {
        discord.m_pLinkedBot = new LevelAPI::Frontend::DiscordInstance(this);
        m_vThreads.push_back(discord.m_pLinkedBot->start());
    }
#endif
#ifdef _TG_ENABLED_
    if (_jsonObject.contains("tgBotToken")) {
        telegram.m_bEnableBot = true;
        GET_JSON_VALUE(_jsonObject, "tgBotToken", telegram.m_sBotToken, std::string);
    }

    if(telegram.m_bEnableBot && !telegram.m_sBotToken.empty()) {
        telegram.m_pLinkedBot = new LevelAPI::Frontend::TelegramInstance(this);
        telegram.m_pLinkedBot->start();
    }
#endif

    return;
}
Database::Database() {
    m_nNodeSize = m_vNodes.size();

    setupJSON();

    _jsonObject["nodes"] = nlohmann::json::array();
    _jsonObject["nodeSize"] = m_nNodeSize;

    save();
}

void Database::recalculate() {
    m_nNodeSize = m_vNodes.size();
}

void Database::save() {
    std::string p1 = databasePath + "/info.json";
    std::string p2 = databasePath + "/info_backup.json";
    if (!m_bLoadedFromBackup) {
        if (std::filesystem::exists(p1)) {
            std::filesystem::copy(p1, p2, std::filesystem::copy_options::overwrite_existing);
        }
    } else {
        if (std::filesystem::exists(p2)) {
            std::filesystem::copy(p2, p1, std::filesystem::copy_options::overwrite_existing);
        }
    }
    //auto start = std::chrono::high_resolution_clock::now();

    recalculate();

    _jsonObject["nodes"] = nlohmann::json::array();
    _jsonObject["nodeSize"] = m_nNodeSize;
#ifdef _DPP_ENABLED_
    if(discord.m_bEnableBot) {
        _jsonObject["tgBotToken"] = telegram.m_sBotToken;
    }
    // _jsonObject["tgRegisteredCID"] = discord.m_sRegisteredCID;
    // _jsonObject["tgRegisteredCID2"] = discord.m_sRegisteredCID2;
#endif

    int i = 0;
    while(i < m_vNodes.size()) {
        m_vNodes.at(i)->save();

        _jsonObject["nodes"].push_back(m_vNodes.at(i)->_jsonObject);
        i++;
    }

    std::ofstream file(p1);
    file << _jsonObject.dump(4);

    //auto end = std::chrono::high_resolution_clock::now();

    //auto etime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    //std::cout << "[LevelAPI] Database took " << (float)etime.count() / 1000.f << "ms to save itself" << std::endl;
}

void Database::setupJSON() {
    _jsonObject = nlohmann::json();
}

Node *Database::getNode(std::string internalName) {
    int i = 0;
    while(i < m_vNodes.size()) {
        if(!m_vNodes.at(i)->m_sInternalName.compare(internalName)) return m_vNodes.at(i);
        i++;
    }
    return nullptr;
}

Database::~Database() {
    int i = 0;

    if (telegram.m_pLinkedBot) {
        telegram.m_pLinkedBot->close();
        delete telegram.m_pLinkedBot;
    }

    while(i < m_vThreads.size()) {
        delete m_vThreads[i];
        m_vThreads[i] = nullptr;
        i++;
    }

    i = 0;
    while(i < m_vNodes.size()) {
        delete m_vNodes.at(i);
        i++;
    }

    m_vNodes.clear();
    m_vThreads.clear();
}

void Database::runThreads() {
    std::thread *dbt = new std::thread(DatabaseController::database_runner, this);
    dbt->detach();

    m_vThreads.push_back(dbt);

    int i = 0;
    while(i < m_vNodes.size()) {
        std::thread *ndt = new std::thread(DatabaseController::node_runner, m_vNodes.at(i));
        m_vThreads.push_back(ndt);
        i++;
    }
}

int Database::getTotalLevels() {
    int i = 0;
    int res = 0;
    while(i < m_vNodes.size()) {
        res += m_vNodes[i]->m_nCachedLevels;
        i++;
    }
    return res;
}
