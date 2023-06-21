#include "GDServer.h"
#include "GDServer_BoomlingsLike19.h"
#include "GDServer_BoomlingsLike21.h"
#include "GDServer_BoomlingsLike22.h"
#include "Level.h"
#include "SearchFilter.h"
#include "gmd2pp/gmd2.h"
#include "lapi_database.h"
#include "json/single_include/nlohmann/json.hpp"
#include <filesystem>
#include <fstream>
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

    setupJSON();

    save();
}
Node::Node(NodeDatabase *database, std::string internalName, std::string levelDataPath) {
    m_uDatabase = database;
    m_sInternalName = internalName;
    m_sLevelDataPath = levelDataPath;
    m_uQueue = new NodeQueue();

    setupJSON();

    save();
}
Node::Node() {
    m_uDatabase = new NodeDatabase();
    m_sInternalName = "node";
    m_sLevelDataPath = "levels";
    m_uQueue = new NodeQueue();
    m_pPolicy = new NodePolicy();
    m_pProxy = new NodeProxyList();

    setupJSON();

    save();
}

void Node::save() {
    m_uDatabase->save();
    m_uQueue->save();
    m_pPolicy->save();
    m_pProxy->save();

    nodeJson["database"] = m_uDatabase->ndJson;
    nodeJson["internalName"] = m_sInternalName;
    nodeJson["levelDataPath"] = m_sLevelDataPath;
    nodeJson["queue"] = m_uQueue->queueJson;
    nodeJson["policy"] = m_pPolicy->policyJson;
    nodeJson["levels"] = m_nCachedLevels;
    nodeJson["experiment1value"] = m_nExperiment1Value;
    nodeJson["proxy"] = m_pProxy->plJson;
}

void Node::recover() {
    GET_JSON_VALUE(nodeJson, "internalName", m_sInternalName, std::string);
    GET_JSON_VALUE(nodeJson, "levelDataPath", m_sLevelDataPath, std::string);
    GET_JSON_VALUE(nodeJson, "experiment1value", m_nExperiment1Value, int);
    m_uQueue->queueJson = nodeJson["queue"];
    m_uQueue->recover();
    m_uDatabase->ndJson = nodeJson["database"];
    m_uDatabase->recover();
    m_pPolicy->policyJson = nodeJson["policy"];
    m_pPolicy->recover();
    m_pProxy->plJson = nodeJson["proxy"];
    m_pProxy->recover();

    std::string p1 = "database/nodes/" + m_sInternalName;
    std::string p2 = "database/nodes/" + m_sInternalName + "/levels";
    std::string p3 = "database/nodes/" + m_sInternalName + "/users";

    mkdir(p1.c_str(), 0777);
    mkdir(p2.c_str(), 0777);
    mkdir(p3.c_str(), 0777);
}

void Node::setupJSON() {
    nodeJson = nlohmann::json();
}

Level *Node::getLevel(int id) {
    
    #define file_exists(cstr) (stat(cstr, &buffer) == 0)

    bool use_gmd2 = false;
    bool nometa = false;

    struct stat buffer;
    std::string p1 = "database/nodes/" + m_sInternalName + "/levels/Level_" + std::to_string(id);
    std::string p2 = "database/nodes/" + m_sInternalName + "/levels/Level_" + std::to_string(id) + "/data.gmd2";
    std::string p3 = "database/nodes/" + m_sInternalName + "/levels/Level_" + std::to_string(id) + "/meta.json";
    std::string p4 = "database/nodes/" + m_sInternalName + "/levels/Level_" + std::to_string(id) + "/raw.txt";
    
    // TODO: comment registering

    use_gmd2 = file_exists(p2.c_str());

    if(!file_exists(p1.c_str()) && !use_gmd2) return nullptr;
    if(!file_exists(p3.c_str()) && !use_gmd2) return nullptr;

    nometa = !file_exists(p3.c_str());

    if (use_gmd2 && nometa) {
        auto ref = new GMD2();
        auto level = new Level();
        ref->setFileName(p2);
        ref->setLevel(level);
        ref->parse();
        level->m_bHasLevelString = true;
        level->m_sLevelPath = p1;
        level->m_nLevelID = id;
        level->m_nVersion = 1;
        level->m_sUsername = "-";
        level->m_uRelease->m_fActualVersion = createServer()->determineGVFromID(id);
        int gv = (int)(std::stof(level->m_uRelease->m_fActualVersion) * 10.f);
        level->m_nGameVersion = gv;
        level->m_uRelease->m_nGameVersion = gv;
        level->save();
        delete ref;
        ref = nullptr;
        return level;
    } else {
        Level *l = new Level();
        try {
            std::ifstream i(p3);
            nlohmann::json file = nlohmann::json::parse(i);
            
            l->levelJson = file;
            l->m_sLevelPath = p1;
            l->restore();

            l->m_bHasLevelString = file_exists(p2.c_str());
            l->m_bHasRawData = file_exists(p4.c_str());

            return l;
        } catch (nlohmann::json::exception &e) {
            delete l;
            l = nullptr;
            return nullptr;
        }
    }
}

Node *Node::getSelf() {
    return this;
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
    std::string p2 = "database/nodes/" + m_sInternalName + "/users/" + std::to_string(level->m_nPlayerID) + ".txt";
    std::string p3 = "database/nodes/" + m_sInternalName + "/users";
    level->m_sLevelPath = p;
    level->m_sLinkedNode = m_sInternalName;
    mkdir(p.c_str(), 0777);
    mkdir(p3.c_str(), 0777);

    if(std::filesystem::exists(p2)) {
        std::fstream f(p2);

        nlohmann::json fj;
        std::vector<int> list;

        if (std::filesystem::file_size(p2) != 0) {
            fj = nlohmann::json::parse(f);
            int i = 0;
            while(i < fj["levels"].size()) {
                int id = fj["levels"].at(i).get<int>();
                
                if(id > 0) {
                    list.push_back(fj["levels"].at(i).get<int>());
                }
                
                i++;
            }
        }

        if(!std::count(list.begin(), list.end(), level->m_nLevelID)) {
            list.push_back(level->m_nLevelID);
        }
        fj["account_id"] = level->m_nAccountID;
        fj["username"] = level->m_sUsername;
        fj["user_id"] = level->m_nPlayerID;
        fj["levels"] = list;

        int size = fj["levels"].size();
        std::cout << "Levels: " << size << " | User ID: " << level->m_nPlayerID << std::endl;
        int i = 0;
        while(i < size) {
            std::cout << "  - Level : " << fj["levels"].at(i).get<int>() << std::endl;
            i++;    
        }

        f << fj.dump();

        f.close();
    } else {
        std::ofstream f(p2);

        nlohmann::json fj;
        std::vector<int> list = { level->m_nLevelID };

        fj["account_id"] = level->m_nAccountID;
        fj["username"] = level->m_sUsername;
        fj["user_id"] = level->m_nPlayerID;
        fj["levels"] = list;

        f << fj.dump();

        f.close();
    }
}
bool Node::userIDExists(int uid) {
    std::string p = "database/nodes/" + m_sInternalName + "/users/" + std::to_string(uid) + ".txt";
    std::string p1 = "database/nodes/" + m_sInternalName + "/users/" + std::to_string(uid) + ".json";

    return std::filesystem::exists(p) || std::filesystem::exists(p1);
}

std::vector<Level *> Node::getLevels(SearchFilter *filter) {
    std::vector<Level *> res = {};
    int i = 0;
    while(i < m_vCachedLevels.size() && i < 8192) {
        bool filter_successful = true;
        auto lvl = getLevel(m_vCachedLevels[i]);
        if(lvl != nullptr) {
            if (filter->m_eFilter != FENone) {
                switch(filter->m_eFilter) {
                    case FEByAccountID: {
                        if(lvl->m_nAccountID == std::stoi(filter->m_sFilterStr)) {
                            if(filter_successful) filter_successful = true;
                        } else {
                            filter_successful = false;
                        }
                        break;
                    }
                    case FEByLevelID: {
                        if(lvl->m_nLevelID == std::stoi(filter->m_sFilterStr)) {
                            if(filter_successful) filter_successful = true;
                        } else {
                            filter_successful = false;
                        }
                        break;
                    }
                    case FEByName: {
                        if(lvl->m_sLevelName.find(filter->m_sFilterStr) != std::string::npos) {
                            filter_successful = true;
                        } else {
                            filter_successful = false;
                        }
                        break;
                    }
                    case FEByDescription: {
                        if(lvl->m_sDescription.find(filter->m_sFilterStr) != std::string::npos) {
                            if(filter_successful) filter_successful = true;
                        } else {
                            filter_successful = false;
                        }
                        break;
                    }
                    case FEByUserID: {
                        if(lvl->m_nPlayerID == std::stoi(filter->m_sFilterStr)) {
                            if(filter_successful) filter_successful = true;
                        } else {
                            filter_successful = false;
                        }
                        break;
                    }
                    case FEByNickname: {
                        if(lvl->m_sUsername.find(filter->m_sFilterStr) != std::string::npos) {
                            if(filter_successful) filter_successful = true;
                        } else {
                            filter_successful = false;
                        }
                        break;
                    }
                    default: {
                        filter_successful = false;
                        break;
                    }
                }
            } else {
                if(filter_successful) filter_successful = true;
            }

            if (filter->m_nDifficulty != -1 && (lvl->m_nDifficultyDenominator == filter->m_nDifficulty)) {
                if(filter_successful) filter_successful = true;
            } else {
                filter_successful = false;
            }
            
            if (filter->m_nStars != -1 && (lvl->m_nStars == filter->m_nStars || lvl->m_nStarsRequested == filter->m_nStars)) {
                if(filter_successful) filter_successful = true;
            } else {
                filter_successful = false;
            }

            if(filter_successful == false) {
                delete lvl;
            } else {
                res.push_back(lvl);
            }
        }
        i++;
    }

    if (filter->m_eSort != SSNone) {
        std::sort(res.begin(), res.end(), [=](Level *lhs, Level *rhs) {
            switch(filter->m_eSort) {
                default:
                case SSMostDownloaded: {
                    return lhs->m_nDownloads > rhs->m_nDownloads;
                    break;
                }
                case SSMostLiked: {
                    return (lhs->m_nLikes + lhs->m_nDislikes) > (rhs->m_nLikes + rhs->m_nDislikes);
                    break;
                }
            }
        });
    }

    return res;
}

LevelAPI::Backend::GDServer *Node::createServer() {
    if(m_pCachedGDInstance != nullptr) return m_pCachedGDInstance;

    LevelAPI::Backend::GDServer *serv;

    switch(m_uDatabase->m_nFeatureSet) {
        case 21: {
            if(m_uDatabase->m_sModifications == "basement-custom") {
                serv = new Backend::GDServer_BasementGDPS(m_uDatabase->m_sEndpoint);
            } else if (m_uDatabase->m_sModifications.empty()) {
                serv = new Backend::GDServer_Boomlings(m_uDatabase->m_sEndpoint);
            } else {
                serv = new Backend::GDServer_BoomlingsLike21(m_uDatabase->m_sEndpoint);
            }
            break;
        }
        case 22: {
            if (m_uDatabase->m_sModifications.empty()) {
                serv = new Backend::GDServer_Boomlings(m_uDatabase->m_sEndpoint);
            } else {
                serv = new Backend::GDServer_BoomlingsLike22(m_uDatabase->m_sEndpoint);
            }
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
                auto level = new Level();
                
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