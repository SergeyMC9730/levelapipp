#include "lapi_database.h"
#include <fstream>
#include <vector>

using namespace LevelAPI::DatabaseController;

Database::Database(std::vector<Node> nodes) {
    m_nNodeSize = nodes.size();
    m_vNodes = nodes;
}
Database::Database(Node node) {
    m_vNodes = std::vector<Node>();
    m_vNodes.push_back(node);
    m_nNodeSize = m_vNodes.size();
}
Database::Database() {
    m_vNodes = std::vector<Node>();
    m_nNodeSize = m_vNodes.size();
}

void Database::recalculate() {
    m_nNodeSize = m_vNodes.size();
}

void Database::load(std::string path) {

}
void Database::save(std::string path) {

}