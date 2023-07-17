#include "DiscordInstance.h"
#include "cluster.h"
#include "dispatcher.h"
#include "intents.h"
#include "lapi_database.h"
#include "json/single_include/nlohmann/json.hpp"
#include "DatabaseControllerThreads.h"
#include "snowflake.h"

#include <filesystem>
#include <fstream>
#include <vector>
#include <iostream>
#include <chrono>
#include <malloc.h>
#include <sys/stat.h>
#include <thread>

#include "Translation.h"
#include "Tools.h"

using namespace LevelAPI::DatabaseController;

bool Database::exists(){ 
    struct stat buffer;
    std::string p = databasePath + "/info.json";
    return (stat (p.c_str(), &buffer) == 0); 
}

Database::Database(std::vector<Node *> nodes) {
    m_nNodeSize = nodes.size();
    m_vNodes = nodes;

    setupJSON();

    _jsonObject["nodes"] = nlohmann::json::array();
    _jsonObject["nodeSize"] = m_nNodeSize;

    save();

    runThreads();
}
Database::Database(Node *node) {
    m_vNodes.push_back(node);
    m_nNodeSize = m_vNodes.size();

    setupJSON();

    _jsonObject["nodes"] = nlohmann::json::array();
    _jsonObject["nodeSize"] = m_nNodeSize;

    save();

    runThreads();
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

    if (_jsonObject.contains("botToken")) {
        m_bEnableBot = true;
        GET_JSON_VALUE(_jsonObject, "botToken", m_sBotToken, std::string);
    }
    GET_JSON_VALUE(_jsonObject, "registeredCID", m_sRegisteredCID, std::string);
    GET_JSON_VALUE(_jsonObject, "registeredCID2", m_sRegisteredCID2, std::string);
    
    GET_JSON_VALUE(_jsonObject, "language", translation_language, std::string);

    if(m_bEnableBot && !m_sBotToken.empty()) {
        m_pLinkedBot = new LevelAPI::DiscordController::DiscordInstance(this);
        m_vThreads.push_back(m_pLinkedBot->start());
    }

    runThreads();

    return;
}
Database::Database() {
    m_nNodeSize = m_vNodes.size();

    setupJSON();

    _jsonObject["nodes"] = nlohmann::json::array();
    _jsonObject["nodeSize"] = m_nNodeSize;

    save();

    runThreads();
}

void Database::recalculate() {
    m_nNodeSize = m_vNodes.size();
}

void Database::save() {
    if (!m_bLoadedFromBackup) {
        std::filesystem::copy(databasePath + "/info.json", databasePath + "/info_backup.json", std::filesystem::copy_options::overwrite_existing);
    } else {
        std::filesystem::copy(databasePath + "/info_backup.json", databasePath + "/info.json", std::filesystem::copy_options::overwrite_existing);
    }
    //auto start = std::chrono::high_resolution_clock::now();

    recalculate();

    _jsonObject["nodes"] = nlohmann::json::array();
    _jsonObject["nodeSize"] = m_nNodeSize;
    if(m_bEnableBot) _jsonObject["botToken"] = m_sBotToken;
    _jsonObject["registeredCID"] = m_sRegisteredCID;
    _jsonObject["registeredCID2"] = m_sRegisteredCID2;

    int i = 0;
    while(i < m_vNodes.size()) {
        m_vNodes.at(i)->save();

        _jsonObject["nodes"].push_back(m_vNodes.at(i)->_jsonObject);
        i++;
    }

    std::string p1 = databasePath + "/info.json";

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