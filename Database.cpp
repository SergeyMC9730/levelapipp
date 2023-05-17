#include "DiscordInstance.h"
#include "cluster.h"
#include "dispatcher.h"
#include "intents.h"
#include "lapi_database.h"
#include "json/single_include/nlohmann/json.hpp"
#include "DatabaseControllerThreads.h"
#include "snowflake.h"

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

    databaseJson["nodes"] = nlohmann::json::array();
    databaseJson["nodeSize"] = m_nNodeSize;

    save();

    runThreads();
}
Database::Database(Node *node) {
    m_vNodes.push_back(node);
    m_nNodeSize = m_vNodes.size();

    setupJSON();

    databaseJson["nodes"] = nlohmann::json::array();
    databaseJson["nodeSize"] = m_nNodeSize;

    save();

    runThreads();
}
Database::Database(std::string path) {
    databasePath = path;

    std::string p1 = path + "/info.json";

    m_nNodeSize = m_vNodes.size();

    if(!exists()) {
        setupJSON();

        databaseJson["nodes"] = nlohmann::json::array();
        databaseJson["nodeSize"] = m_nNodeSize;

        save();

        return;
    }

    std::ifstream i(p1);
    databaseJson = nlohmann::json::parse(i);

    GET_JSON_VALUE(databaseJson, "nodeSize", m_nNodeSize, int);
    int ii = 0;
    while(ii < databaseJson["nodes"].size()) {
        m_vNodes.push_back(new Node());
        m_vNodes.at(ii)->nodeJson = databaseJson["nodes"].at(ii);
        m_vNodes.at(ii)->recover();
        ii++;
    }

    recalculate();

    if (databaseJson.contains("botToken")) {
        m_bEnableBot = true;
        GET_JSON_VALUE(databaseJson, "botToken", m_sBotToken, std::string);
    }
    GET_JSON_VALUE(databaseJson, "registeredCID", m_sRegisteredCID, std::string);
    GET_JSON_VALUE(databaseJson, "registeredCID2", m_sRegisteredCID2, std::string);
    
    GET_JSON_VALUE(databaseJson, "language", translation_language, std::string);

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

    databaseJson["nodes"] = nlohmann::json::array();
    databaseJson["nodeSize"] = m_nNodeSize;

    save();

    runThreads();
}

void Database::recalculate() {
    m_nNodeSize = m_vNodes.size();
}

void Database::save() {
    //auto start = std::chrono::high_resolution_clock::now();

    recalculate();

    databaseJson["nodes"] = nlohmann::json::array();
    databaseJson["nodeSize"] = m_nNodeSize;
    if(m_bEnableBot) databaseJson["botToken"] = m_sBotToken;
    databaseJson["registeredCID"] = m_sRegisteredCID;
    databaseJson["registeredCID2"] = m_sRegisteredCID2;

    int i = 0;
    while(i < m_vNodes.size()) {
        m_vNodes.at(i)->save();

        databaseJson["nodes"].push_back(m_vNodes.at(i)->nodeJson);
        i++;
    }

    std::string p1 = databasePath + "/info.json";

    std::ofstream file(p1);
    file << databaseJson.dump(4);

    int r = malloc_trim(0);
    if(!r) {
        std::cout << "[LevelAPI WARN] Couldn't clean up memory" << std::endl;
    }

    //auto end = std::chrono::high_resolution_clock::now();

    //auto etime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    //std::cout << "[LevelAPI] Database took " << (float)etime.count() / 1000.f << "ms to save itself" << std::endl;
}

void Database::setupJSON() {
    databaseJson = nlohmann::json();
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