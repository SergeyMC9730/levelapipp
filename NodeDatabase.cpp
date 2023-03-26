#include "lapi_database.h"

using namespace LevelAPI::DatabaseController;

NodeDatabase::NodeDatabase(std::string *endpoint, uint8_t featureSet, bool readOnly) {
    m_sEndpoint = endpoint;
    m_nFeatureSet = featureSet;
    m_bReadOnly = readOnly;

    setupJSON();

    save();
}
NodeDatabase::NodeDatabase() {
    m_sEndpoint = new std::string("");
    m_nFeatureSet = 21;
    m_bReadOnly = true;

    setupJSON();
    save();
}

void NodeDatabase::save() {
    ndJson["endpoint"] = *m_sEndpoint;
    ndJson["featureSet"] = m_nFeatureSet;
    ndJson["readOnly"] = m_bReadOnly;
}

void NodeDatabase::recover() {
    m_sEndpoint = new std::string(ndJson["endpoint"].get<std::string>());
    m_nFeatureSet = ndJson["featureSet"].get<int>();
    m_bReadOnly = ndJson["readOnly"].get<bool>();
}

void NodeDatabase::setupJSON() {
    ndJson = nlohmann::json();
}

NodeDatabase::~NodeDatabase() {
    delete m_sEndpoint;
    m_sEndpoint = nullptr;
}