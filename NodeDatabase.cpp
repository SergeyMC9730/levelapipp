#include "lapi_database.h"

using namespace LevelAPI::DatabaseController;

NodeDatabase::NodeDatabase(std::string endpoint, uint8_t featureSet, bool readOnly) {
    m_sEndpoint = endpoint;
    m_nFeatureSet = featureSet;
    m_bReadOnly = readOnly;

    setupJSON();

    save();
}
NodeDatabase::NodeDatabase() {
    m_nFeatureSet = 21;
    m_bReadOnly = true;

    setupJSON();
    save();
}

void NodeDatabase::save() {
    ndJson["endpoint"] = m_sEndpoint;
    ndJson["featureSet"] = m_nFeatureSet;
    ndJson["readOnly"] = m_bReadOnly;
    if(!m_sModifications.empty()) ndJson["modifications"] = m_sModifications;
}

void NodeDatabase::recover() {
    m_sEndpoint = ndJson["endpoint"].get<std::string>();
    m_nFeatureSet = ndJson["featureSet"].get<int>();
    m_bReadOnly = ndJson["readOnly"].get<bool>();
    if(ndJson.contains("modifications")) m_sModifications = ndJson["modifications"].get<std::string>();
}

void NodeDatabase::setupJSON() {
    ndJson = nlohmann::json();
}

NodeDatabase::~NodeDatabase() {}

std::vector<std::string> NodeDatabase::getModifications() {
    // wip
    return {m_sModifications};
}