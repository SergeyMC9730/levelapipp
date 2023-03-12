#include "lapi_database.h"
#include "json/single_include/nlohmann/json.hpp"
#include <string>
#include <sys/stat.h>

using namespace LevelAPI::DatabaseController;

Node::Node(NodeDatabase database, std::string internalName, std::string levelDataPath, NodeQueue queue) {
    m_uDatabase = database;
    m_sInternalName = internalName;
    m_sLevelDataPath = levelDataPath;
    m_uQueue = queue;

    setupJSON();

    nodeJson["database"] = database.ndJson;
    nodeJson["internalName"] = internalName;
    nodeJson["levelDataPath"] = levelDataPath;
    nodeJson["queue"] = m_uQueue.queueJson;
}
Node::Node(NodeDatabase database, std::string internalName, std::string levelDataPath) {
    m_uDatabase = database;
    m_sInternalName = internalName;
    m_sLevelDataPath = levelDataPath;
    m_uQueue = NodeQueue();

    setupJSON();

    nodeJson["database"] = m_uDatabase.ndJson;
    nodeJson["internalName"] = internalName;
    nodeJson["levelDataPath"] = levelDataPath;
    nodeJson["queue"] = m_uQueue.queueJson;
}
Node::Node() {
    m_uDatabase = NodeDatabase();
    m_sInternalName = "-";
    m_sLevelDataPath = "-";
    m_uQueue = NodeQueue();

    setupJSON();

    nodeJson["database"] = m_uDatabase.ndJson;
    nodeJson["internalName"] = m_sInternalName;
    nodeJson["levelDataPath"] = m_sLevelDataPath;
    nodeJson["queue"] = m_uQueue.queueJson;
}

void Node::save() {
    m_uDatabase.save();
    m_uQueue.save();

    nodeJson["database"] = m_uDatabase.ndJson;
    nodeJson["internalName"] = m_sInternalName;
    nodeJson["levelDataPath"] = m_sLevelDataPath;
    nodeJson["queue"] = m_uQueue.queueJson;
}

void Node::recover() {
    m_sInternalName = nodeJson["internalName"].get<std::string>();
    m_sLevelDataPath = nodeJson["levelDataPath"].get<std::string>();
    m_uQueue.queueJson = nodeJson["queue"];
    m_uQueue.recover();
    m_uDatabase.ndJson = nodeJson["database"];
    m_uDatabase.recover();

    std::string p1 = "database/nodes/" + m_sInternalName;
    std::string p2 = "database/nodes/" + m_sInternalName + "/levels";

    mkdir(p1.c_str(), 0777);
    mkdir(p2.c_str(), 0777);
}

void Node::setupJSON() {
    nodeJson = nlohmann::json();
}

Level Node::getLevel(int id) {
    #define file_exists(cstr) (stat(cstr, &buffer) == 0)

    struct stat buffer;
    std::string p1 = "database/nodes/" + m_sInternalName + "/levels/" + std::to_string(id);
    std::string p2 = "database/nodes/" + m_sInternalName + "/levels/" + std::to_string(id) + "/level.gmd2";
    std::string p3 = "database/nodes/" + m_sInternalName + "/levels/" + std::to_string(id) + "/meta.json";
    // TODO comment registering
    
    if(!file_exists(p1.c_str())) return Level();
    if(!file_exists(p2.c_str())) return Level();
    if(!file_exists(p3.c_str())) return Level();

    nlohmann::json file = nlohmann::json::parse(p3);
    
    Level l = Level();
    l.levelJson = file;
    l.m_sLevelPath = p1;
    l.restore();

    return l;
}

Node *Node::getSelf() {
    return this;
}