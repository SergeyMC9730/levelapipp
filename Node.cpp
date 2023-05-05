#include "GDServer.h"
#include "GDServer_BoomlingsLike19.h"
#include "GDServer_BoomlingsLike21.h"
#include "SearchFilter.h"
#include "lapi_database.h"
#include "json/single_include/nlohmann/json.hpp"
#include <string>
#include <sys/stat.h>
#include <vector>
#include <algorithm>
#include "GDServer_Boomlings.h"
#include "GDServer_BasementGDPS.h"
#include "GDServer_19GDPS.h"
#include "Translation.h"

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

    setupJSON();

    save();
}

void Node::save() {
    m_uDatabase->save();
    m_uQueue->save();
    m_pPolicy->save();

    nodeJson["database"] = m_uDatabase->ndJson;
    nodeJson["internalName"] = m_sInternalName;
    nodeJson["levelDataPath"] = m_sLevelDataPath;
    nodeJson["queue"] = m_uQueue->queueJson;
    nodeJson["policy"] = m_pPolicy->policyJson;
    nodeJson["levels"] = m_vCachedLevels.size();
}

void Node::recover() {
    m_sInternalName = nodeJson["internalName"].get<std::string>();
    m_sLevelDataPath = nodeJson["levelDataPath"].get<std::string>();
    m_uQueue->queueJson = nodeJson["queue"];
    m_uQueue->recover();
    m_uDatabase->ndJson = nodeJson["database"];
    m_uDatabase->recover();
    m_pPolicy->policyJson = nodeJson["policy"];
    m_pPolicy->recover();

    std::string p1 = "database/nodes/" + m_sInternalName;
    std::string p2 = "database/nodes/" + m_sInternalName + "/levels";

    mkdir(p1.c_str(), 0777);
    mkdir(p2.c_str(), 0777);
}

void Node::setupJSON() {
    nodeJson = nlohmann::json();
}

Level *Node::getLevel(int id) {
    
    #define file_exists(cstr) (stat(cstr, &buffer) == 0)

    struct stat buffer;
    std::string p1 = "database/nodes/" + m_sInternalName + "/levels/Level_" + std::to_string(id);
    std::string p2 = "database/nodes/" + m_sInternalName + "/levels/Level_" + std::to_string(id) + "/data.gmd2";
    std::string p3 = "database/nodes/" + m_sInternalName + "/levels/Level_" + std::to_string(id) + "/meta.json";
    // TODO: comment registering
    
    if(!file_exists(p1.c_str())) return nullptr;
    if(!file_exists(p3.c_str())) return nullptr;

    std::ifstream i(p3);
    nlohmann::json file = nlohmann::json::parse(i);
    
    Level *l = new Level();
    l->levelJson = file;
    l->m_sLevelPath = p1;
    l->restore();

    l->m_bHasLevelString = file_exists(p2.c_str());

    return l;
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
}

void Node::initLevel(Level *level) {
    std::string p = "database/nodes/" + m_sInternalName + "/levels/Level_" + std::to_string(level->m_nLevelID);
    level->m_sLevelPath = p;
    mkdir(p.c_str(), 0777);
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
    LevelAPI::Backend::GDServer *serv;

    switch(m_uDatabase->m_nFeatureSet) {
        case 21: {
            if(m_uDatabase->m_sModifications == "basement-custom") {
                serv = new Backend::GDServer_BasementGDPS(m_uDatabase->m_sEndpoint);
            } else if (m_uDatabase->m_sModifications == "") {
                serv = new Backend::GDServer_Boomlings(m_uDatabase->m_sEndpoint);
            } else {
                serv = new Backend::GDServer_BoomlingsLike21(m_uDatabase->m_sEndpoint);
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

    std::cout << Frontend::Translation::getByKey("lapi.node.selected_server", m_sInternalName, serv->getServerName()) << std::endl;

    return serv;
}