#include "GDServer.h"
#include "GDServer_BoomlingsLike19.h"
#include "GDServer_BoomlingsLike21.h"
#include "GDServer_BoomlingsLike22.h"
#include "Level.h"
#include "SQLiteManager.h"
#include "SearchFilter.h"
#include "gmd2pp/gmd2.h"
#include "lapi_database.h"
#include "json/single_include/nlohmann/json.hpp"
#include <filesystem>
#include <fstream>
#include <new>
#include <string>
#include <sys/stat.h>
#include <vector>
#include <algorithm>
#include "GDServer_Boomlings.h"
#include "GDServer_BasementGDPS.h"
#include "GDServer_19GDPS.h"
#include "Translation.h"
#include "Tools.h"

using namespace LevelAPI::DatabaseController;

Node::Node(NodeDatabase *database, std::string internalName, std::string levelDataPath, NodeQueue *queue) {
    m_uDatabase = database;
    m_sInternalName = internalName;
    m_sLevelDataPath = levelDataPath;
    m_uQueue = queue;

    createLevelFolder();

    setupJSON();
    setupSQLite();

    save();
}
Node::Node(NodeDatabase *database, std::string internalName, std::string levelDataPath) {
    m_uDatabase = database;
    m_sInternalName = internalName;
    m_sLevelDataPath = levelDataPath;
    m_uQueue = new NodeQueue();

    createLevelFolder();

    setupJSON();
    setupSQLite();

    save();
}
Node::Node(std::string internalName) {
    m_uDatabase = new NodeDatabase();
    m_sInternalName = internalName;
    m_sLevelDataPath = "levels";
    m_uQueue = new NodeQueue();
    m_pPolicy = new NodePolicy();
    m_pProxy = new NodeProxyList();

    createLevelFolder();

    setupJSON();
    setupSQLite();

    save();
}

void Node::setupSQLite() {
    std::string path = "database/nodes/" + m_sInternalName + "/information.db";

    if (!std::filesystem::exists(path)) {
        std::filesystem::copy("resources/nodetemplate.db", path);
    }

    _sqliteObject = SQLiteManager::create(path);
}

bool Node::levelExists(int id) {
    std::string path = "database/nodes/" + m_sInternalName + "/levels/Level_" + std::to_string(id);

    return std::filesystem::exists(path);
}

void Node::createLevelFolder() {
    std::string p1 = "database/nodes/" + m_sInternalName;
    std::string p2 = "database/nodes/" + m_sInternalName + "/levels";

    mkdir(p1.c_str(), 0777);
    mkdir(p2.c_str(), 0777);
}

void Node::save() {
    m_uDatabase->save();
    m_uQueue->save();
    m_pPolicy->save();
    m_pProxy->save();

    _jsonObject["database"] = m_uDatabase->_jsonObject;
    _jsonObject["internalName"] = m_sInternalName;
    _jsonObject["levelDataPath"] = m_sLevelDataPath;
    // if (_jsonObject["queue"].size() != 0) {
	//     //_jsonObject["queue"].erase(_jsonObject["queue"].begin(), _jsonObject["queue"].end()); 
    // }
    // if (_jsonObject["queue"] != nullptr) {
        try {
            // _jsonObject["queue"] = {};
            // _jsonObject["queue"].clear();
            _jsonObject["queue"] = m_uQueue->_jsonObject;
        } catch (std::bad_alloc e) {
            std::cout << "allocation failed" << std::endl;
            _jsonObject["queue"] = nlohmann::json();
        }
    // } else {
        // std::cout << "queue is nullptr" << std::endl;
        // nlohmann::json j;
        // j["commandQueue"] = {};
        // j["executeQueue"] = false;
        // j["runtimeState"] = 0;
        // _jsonObject["queue"] = j;
    // }
    _jsonObject["policy"] = m_pPolicy->_jsonObject;
    _jsonObject["levels"] = m_nCachedLevels;
    _jsonObject["experiment1value"] = m_nExperiment1Value;
    _jsonObject["proxy"] = m_pProxy->_jsonObject;
}

void Node::recover() {
    GET_JSON_VALUE(_jsonObject, "internalName", m_sInternalName, std::string);
    GET_JSON_VALUE(_jsonObject, "levelDataPath", m_sLevelDataPath, std::string);
    GET_JSON_VALUE(_jsonObject, "experiment1value", m_nExperiment1Value, int);
    m_uQueue->_jsonObject = _jsonObject["queue"];
    m_uQueue->recover();
    m_uDatabase->_jsonObject = _jsonObject["database"];
    m_uDatabase->recover();
    m_pPolicy->_jsonObject = _jsonObject["policy"];
    m_pPolicy->recover();
    m_pProxy->_jsonObject = _jsonObject["proxy"];
    m_pProxy->recover();

    std::string p1 = "database/nodes/" + m_sInternalName;
    std::string p2 = "database/nodes/" + m_sInternalName + "/levels";

    mkdir(p1.c_str(), 0777);
    mkdir(p2.c_str(), 0777);
}

void Node::setupJSON() {
    _jsonObject = nlohmann::json();
}

nlohmann::json Node::jsonFromSQLLevel(SQLiteServerRow row) {
    nlohmann::json obj;

    obj["levelID"] = std::stoi(row["levelID"]);
    obj["version"] = std::stoi(row["version"]);
    obj["playerID"] = std::stoi(row["playerID"]);
    obj["downloads"] = std::stoi(row["downloads"]);
    obj["musicID"] = std::stoi(row["musicID"]);
    obj["likes"] = std::stoi(row["likes"]);
    obj["length"] = std::stoi(row["length"]);
    obj["difficulty_denominator"] = std::stoi(row["difficulty_denominator"]);
    obj["fakeGameVersion"] = std::stoi(row["fakeGameVersion"]);
    obj["dislikes"] = std::stoi(row["dislikes"]);
    obj["stars"] = std::stoi(row["stars"]);
    obj["featureScore"] = std::stoi(row["featureScore"]);
    obj["copiedFrom"] = std::stoi(row["copiedFrom"]);
    obj["dailyNumber"] = std::stoi(row["dailyNumber"]);
    obj["coins"] = std::stoi(row["coins"]);
    obj["starsRequested"] = std::stoi(row["starsRequested"]);
    obj["isEpic"] = std::stoi(row["isEpic"]);
    obj["demonDifficulty"] = std::stoi(row["demonDifficulty"]);
    obj["editorTime"] = std::stoi(row["editorTime"]);
    obj["editorTimeTotal"] = std::stoi(row["editorTimeTotal"]);
    obj["accountID"] = std::stoi(row["accountID"]);
    obj["songID"] = std::stoi(row["songID"]);
    obj["objects"] = std::stoi(row["objects"]);
    obj["moons"] = std::stoi(row["moons"]);

    obj["appereanceTimestamp"] = std::stoi(row["databaseAppereanceDate"]);
   
    obj["isAuto"] = (bool)std::stoi(row["isAuto"]);
    obj["isDemon"] = (bool)std::stoi(row["isDemon"]);
    obj["areCoinsVerified"] = (bool)std::stoi(row["areCoinsVerified"]);
    obj["ldmAvailable"] = (bool)std::stoi(row["ldmAvailable"]);
    obj["is2P"] = (bool)std::stoi(row["is2P"]);

    obj["levelName"] = row["levelName"];
    obj["levelDescription"] = row["levelDescription"];
    obj["uploadDate"] = row["uploadDate"];
    obj["updateDate"] = row["updateDate"];
    obj["username"] = row["username"];
    obj["actualGameVersion"] = row["actualGameVersion"];

    return obj;
}

Level *Node::getLevel(int id) {
    auto lvl = _sqliteObject->getTableWithCondition("levels", "levelID", 10, 1, {
        {"levelID", id}
    });

    std::string p1 = "database/nodes/" + m_sInternalName + "/levels/Level_" + std::to_string(id);

    if (lvl.size() == 0) {
        // try to load gmd2 data idk

        std::string p2 = "database/nodes/" + m_sInternalName + "/levels/Level_" + std::to_string(id) + "/data.gmd2";

        if (!std::filesystem::exists(p2)) return nullptr; // no level data at all!

        auto ref = new GMD2();
        auto level = new Level(m_sInternalName);

        ref->setFileName(p2);
        ref->setLevel(level);
        ref->parse();

        int gv = (int)(std::stof(level->m_uRelease->m_fActualVersion) * 10.f);

        level->m_bHasLevelString = true;
        level->m_sLinkedNode = m_sInternalName;
        level->m_sLevelPath = p1;
        level->m_nLevelID = id;
        level->m_nVersion = 1;
        level->m_sUsername = "-";
        level->m_uRelease->m_fActualVersion = createServer()->determineGVFromID(id);
        level->m_nGameVersion = gv;
        level->m_uRelease->m_nGameVersion = gv;

        initLevel(level);
        level->save();
        
        delete ref;
        ref = nullptr;
        return level;
    } else {
        auto level = new Level(m_sInternalName);
        auto sqlobj = lvl[0];

        level->_jsonObject = jsonFromSQLLevel(sqlobj);

        level->m_sLevelPath = p1;
        level->m_sLinkedNode = m_sInternalName;
        level->recover();

        return level;
    }

    return nullptr;
}

Node::~Node() {
    delete m_uDatabase;
    m_uDatabase = nullptr;
    delete m_uQueue;
    m_uQueue = nullptr;
    delete m_pPolicy;
    m_pPolicy = nullptr;
    delete m_pProxy;
    m_pProxy = nullptr;
}

int initLevel_min = 0;
int initLevel_max = 0;

void Node::initLevel(Level *level) {
    std::string p = "database/nodes/" + m_sInternalName + "/levels/Level_" + std::to_string(level->m_nLevelID);
    // std::string p2 = "database/nodes/" + m_sInternalName + "/users/" + std::to_string(level->m_nPlayerID) + ".txt";
    // std::string p3 = "database/nodes/" + m_sInternalName + "/users";
    level->m_sLevelPath = p;
    level->m_sLinkedNode = m_sInternalName;
    mkdir(p.c_str(), 0777);
    // mkdir(p3.c_str(), 0777);

    auto user = _sqliteObject->getTableWithCondition("accounts", "userID", 10, 1, {
        {"userID", level->m_nPlayerID}
    });

    if (user.size() == 0) {
        nlohmann::json j = {level->m_nLevelID};

        _sqliteObject->pushRow({
            {"accountID", level->m_nAccountID},
            {"username", level->m_sUsername},
            {"levels", "[]"},
            {"userID", level->m_nPlayerID}
        }, "accounts");
    }
}
bool Node::userIDExists(int uid) {
    auto user = _sqliteObject->getTableWithCondition("accounts", "userID", 10, 1, {
        {"userID", uid}
    });

    return user.size() >= 1;
}

std::vector<Level *> Node::getLevels(SearchFilter *filter) {
    std::vector<Level *> res = {};

    SQLiteRow rw_condition = {};

    if (filter->m_nStars != -1) {
        rw_condition["stars"] = filter->m_nStars;
    }
    if (filter->m_nDifficulty != -1) {
        rw_condition["difficulty_numenator"] = filter->m_nDifficulty;
    }

    if (!filter->m_sName.empty()) {
        rw_condition["levelName"] = filter->m_sName;
    }
    if (!filter->m_sDescription.empty()) {
        rw_condition["levelDescription"] = filter->m_sName;
    }

    if (filter->m_nUID != 0) {
        rw_condition["playerID"] = filter->m_nUID;
    }
    if (filter->m_nAID != 0) {
        rw_condition["accountID"] = filter->m_nAID;
    }

    if (!filter->m_sUsername.empty()) {
        rw_condition["username"] = filter->m_sUsername;
    }

    if (filter->m_nSID != -1) {
        if (filter->m_bSongOfficial) {
            rw_condition["musicID"] = filter->m_nSID;
        } else {
            rw_condition["songID"] = filter->m_nSID;
        }
    }

    if (filter->m_nServerGV != 0) {
        rw_condition["fakeGameVersion"] = filter->m_nServerGV;
    }
    if (!filter->m_sReleaseGV.empty()) {
        rw_condition["actualGameVersion"] = filter->m_sReleaseGV;
    }

    std::string ordering = "-downloads";

    if (filter->m_eSort == SearchSort::SSMostLiked) {
        ordering = "-likes";
    }
    if (filter->m_eSort == SearchSort::SSMostDownloaded) {
        ordering = "-downloads";
    }
    if (filter->m_eSort == SearchSort::SSLatestDBApperead) {
        ordering = "-databaseAppereanceDate";
    }
    if (filter->m_eSort == SearchSort::SSRecentLevel) {
        ordering = "-levelID";
    }
    if (filter->m_eSort == SearchSort::SSOldestLevel) {
        ordering = "levelID";
    }

    auto request = _sqliteObject->getTableWithCondition("levels", ordering, filter->m_nLevelsPerPage, filter->m_nPage, rw_condition);

    int i = 0;

    while(i < request.size()) {
        auto sqlobj = request[i];
        auto jsonobj = jsonFromSQLLevel(sqlobj);
        auto level = new Level(m_sInternalName);

        level->_jsonObject = jsonobj;
        level->m_sLinkedNode = m_sInternalName;
        level->recover();

        res.push_back(level);

        i++;
    }

    return res;
}

LevelAPI::Backend::GDServer *Node::createServer() {
    if(m_pCachedGDInstance != nullptr) return m_pCachedGDInstance;

    LevelAPI::Backend::GDServer *serv = nullptr;

    switch(m_uDatabase->m_nFeatureSet) {
        case 21: {
            if(m_uDatabase->m_sModifications == "basement-custom") {
                serv = new Backend::GDServer_BasementGDPS(m_uDatabase->m_sEndpoint);
            } else if (m_uDatabase->m_sModifications.empty() || m_uDatabase->m_sModifications == "boomlings") {
                serv = new Backend::GDServer_Boomlings(m_uDatabase->m_sEndpoint);
            } else {
                serv = new Backend::GDServer_BoomlingsLike21(m_uDatabase->m_sEndpoint);
            }
            break;
        }
        case 22: {
            if (m_uDatabase->m_sModifications.empty() || m_uDatabase->m_sModifications == "boomlings") {
                serv = new Backend::GDServer_Boomlings(m_uDatabase->m_sEndpoint);
            } else {
                serv = new Backend::GDServer_BoomlingsLike22(m_uDatabase->m_sEndpoint);
            }

            break;
        }
        case 19: {
            if(m_uDatabase->m_sModifications == "19gdps-custom") {
                serv = new Backend::GDServer_19GDPS(m_uDatabase->m_sEndpoint);
            } else {
                serv = new Backend::GDServer_BoomlingsLike19(m_uDatabase->m_sEndpoint);
            }
            break;
        }
        default: {
            serv = new Backend::GDServer();
            break;
        }
    }

    if(!m_uDatabase->m_sPlayerPassword.empty()) {
        serv->setCredentials(m_uDatabase->m_sPlayerLogin, m_uDatabase->m_sPlayerPassword);
    }
    serv->setDebug(true);
    m_pCachedGDInstance = serv;

    std::cout << Frontend::Translation::getByKey("lapi.node.selected_server", m_sInternalName, serv->getServerName()) << std::endl;

    return serv;
}

void Node::importLevelMetaFromLAPIold(std::string p) {
    std::thread thrd ([=](Node *self, std::string path){
        #define file_exists(cstr) (stat(cstr, &buffer) == 0)

        struct stat buffer;

        if(!file_exists(path.c_str())) return;
        std::cout << Frontend::Translation::getByKey("lapi.node.import.open.start", path) << std::endl;

        std::ifstream i(path);
        std::cout << Frontend::Translation::getByKey("lapi.node.import.ifstream", path) << std::endl;
        nlohmann::json file = nlohmann::json::parse(i);
        std::cout << Frontend::Translation::getByKey("lapi.node.import.open.end", path) << std::endl;

        auto j1 = file.at(2);
        auto j2 = j1["data"];
        if(!j2.is_array()) {
            i.close();
            return;
        }

        int levels = 0;
        int levels_from_f = 0;
        
        int q = 0;
        while(q < j2.size()) {
            auto obj = j2.at(q);
            std::string nd = obj["node"].get<std::string>();
            if(nd == m_sInternalName) {
                levels++;
                int id = std::stoi(obj["id"].get<std::string>());
                int gv = std::stoi(obj["gameVersion"].get<std::string>());
                std::string lname = obj["name"].get<std::string>();
                std::string ldesc = obj["description"].get<std::string>();
                int downloads = std::stoi(obj["levelDownloads"].get<std::string>());
                int likes = std::stoi(obj["levelLikes"].get<std::string>());
                std::string nickname = "-";
                int accountID = 0;
                if(!obj["authorNickname"].is_null()) {
                    nickname = obj["authorNickname"].get<std::string>();
                }
                if(!obj["authorAccountID"].is_null()) {
                    accountID = std::stoi(obj["authorAccountID"].get<std::string>());
                }
                int userID = std::stoi(obj["authorUserID"].get<std::string>());
                bool musicIsOfficial = std::stoi(obj["musicIsOfficial"].get<std::string>());
                int musicID = std::stoi(obj["musicID"].get<std::string>());
                int rating = std::stoi(obj["difficultyRating"].get<std::string>());
                bool isDemon = std::stoi(obj["isDemon"].get<std::string>());
                bool commentSectionAvailable = std::stoi(obj["commentSectionAvailable"].get<std::string>());
                auto level = new Level(m_sInternalName);
                
                if(accountID == -1) accountID = 0;
                
                level->m_bHasLevelString = false;
                level->m_nAccountID = accountID;
                level->m_sLinkedNode = nd;
                level->m_sUsername = nickname;
                level->m_uRelease->m_nGameVersion = gv;
                level->m_uRelease->m_fActualVersion = createServer()->determineGVFromID(id);
                level->m_nGameVersion = gv;
                level->m_nLevelID = id;
                level->m_sLevelName = lname;
                level->m_nDifficultyDenominator = rating;
                level->m_nLikes = likes;
                level->m_nDownloads = downloads;
                level->m_nVersion = 1;
                if(musicIsOfficial) level->m_nMusicID = musicID;
                else {
                    level->m_nSongID = musicID;
                }
                initLevel(level);
                level->save();
                std::cout << Frontend::Translation::getByKey("lapi.node.import.level", id, lname) << std::endl;
                delete level;
            } else {
                levels_from_f++;
            }
            q++;
        }

        std::cout << Frontend::Translation::getByKey("lapi.node.import.end1", levels) << std::endl;
        std::cout << Frontend::Translation::getByKey("lapi.node.import.end2", levels_from_f) << std::endl;

        i.close();
    }, this, p);
    thrd.detach();
}
