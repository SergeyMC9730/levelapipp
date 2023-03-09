#include "lapi_database.h"

using namespace LevelAPI::DatabaseController;


Node::Node(NodeDatabase database, std::string internalName, std::string levelDataPath, NodeQueue queue) {
    m_uDatabase = database;
    m_sInternalName = internalName;
    m_sLevelDataPath = levelDataPath;
    m_uQueue = queue;
}
Node::Node(NodeDatabase database, std::string internalName, std::string levelDataPath) {
    m_uDatabase = database;
    m_sInternalName = internalName;
    m_sLevelDataPath = levelDataPath;
    m_uQueue = NodeQueue();
}
Node::Node() {
    m_uDatabase = NodeDatabase();
    m_sInternalName = "-";
    m_sLevelDataPath = "-";
    m_uQueue = NodeQueue();
}