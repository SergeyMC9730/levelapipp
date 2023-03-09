#include "lapi_database.h"

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
}

void Node::setupJSON() {
    nodeJson = nlohmann::json();
}