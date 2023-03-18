#include "lapi_database.h"
#include "json/single_include/nlohmann/json.hpp"
#include <fstream>
#include <vector>
#include <iostream>
#include <chrono>
#include <malloc.h>

#include <sys/stat.h>

using namespace LevelAPI::DatabaseController;

bool Database::exists(){ 
    struct stat buffer;
    std::string p = *databasePath + "/info.json";
    return (stat (p.c_str(), &buffer) == 0); 
}

Database::Database(std::vector<Node *> *nodes) {
    m_nNodeSize = nodes->size();
    m_vNodes = nodes;

    setupJSON();

    databaseJson["nodes"] = nlohmann::json::array();
    databaseJson["nodeSize"] = m_nNodeSize;

    save();
}
Database::Database(Node *node) {
    m_vNodes = new std::vector<Node *>();
    m_vNodes->push_back(node);
    m_nNodeSize = m_vNodes->size();

    setupJSON();

    databaseJson["nodes"] = nlohmann::json::array();
    databaseJson["nodeSize"] = m_nNodeSize;

    save();
}
Database::Database(std::string *path) {
    databasePath = path;

    std::string p1 = *path + "/info.json";

    m_vNodes = new std::vector<Node *>();
    m_nNodeSize = m_vNodes->size();

    if(!exists()) {
        setupJSON();

        databaseJson["nodes"] = nlohmann::json::array();
        databaseJson["nodeSize"] = m_nNodeSize;

        save();

        return;
    }

    std::ifstream i(p1);
    databaseJson = nlohmann::json::parse(i);

    m_nNodeSize = databaseJson["nodeSize"].get<int>();
    int ii = 0;
    while(ii < databaseJson["nodes"].size()) {
        m_vNodes->push_back(new Node());
        m_vNodes->at(ii)->nodeJson = databaseJson["nodes"].at(ii);
        m_vNodes->at(ii)->recover();
        ii++;
    }

    recalculate();

    return;
}
Database::Database() {
    m_vNodes = new std::vector<Node *>();
    m_nNodeSize = m_vNodes->size();

    setupJSON();

    databaseJson["nodes"] = nlohmann::json::array();
    databaseJson["nodeSize"] = m_nNodeSize;

    save();
}

void Database::recalculate() {
    m_nNodeSize = m_vNodes->size();
}

void Database::save() {
    auto start = std::chrono::high_resolution_clock::now();

    recalculate();

    databaseJson["nodes"] = nlohmann::json::array();
    databaseJson["nodeSize"] = m_nNodeSize;

    int i = 0;
    while(i < m_vNodes->size()) {
        m_vNodes->at(i)->save();

        databaseJson["nodes"].push_back(m_vNodes->at(i)->nodeJson);
        i++;
    }

    std::string p1 = *databasePath + "/info.json";

    std::ofstream file(p1);
    file << databaseJson.dump(4);

    int r = malloc_trim(0);
    if(!r) {
        std::cout << "[LevelAPI WARN] Couldn't clean up memory" << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto etime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "[LevelAPI] Database took " << (float)etime.count() / 1000.f << "ms to save itself" << std::endl;
}

void Database::setupJSON() {
    databaseJson = nlohmann::json();
}

Node *Database::getNode(std::string internalName) {
    int i = 0;
    while(i < m_vNodes->size()) {
        if(!m_vNodes->at(i)->m_sInternalName->compare(internalName)) return m_vNodes->at(i);
        i++;
    }
    return nullptr;
}

Database::~Database() {
    printf("~database\n");
    int i = 0;
    while(i < m_vNodes->size()) {
        delete m_vNodes->at(i);
        i++;
    }
    delete m_vNodes;
    delete databasePath;
}