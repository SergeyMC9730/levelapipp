#include "lapi_database.h"

using namespace LevelAPI::DatabaseController;

NodeDatabase::NodeDatabase(std::string *endpoint, uint8_t featureSet, bool readOnly) {
    m_sEndpoint = endpoint;
    m_nFeatureSet = featureSet;
    m_bReadOnly = readOnly;

    ndJson["endpoint"] = *endpoint;
    ndJson["featureSet"] = featureSet;
    ndJson["readOnly"] = readOnly;
}
NodeDatabase::NodeDatabase() {
    NodeDatabase(new std::string(""), 21, true);
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
    printf("~nodedatabase\n");
    delete m_sEndpoint;
}