#include "lapi_database.h"
#include "Tools.h"

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
    ndJson["playerLogin"] = m_sPlayerLogin;
    ndJson["playerPassword"] = m_sPlayerPassword;
    if(!m_sModifications.empty()) ndJson["modifications"] = m_sModifications;
}

void NodeDatabase::recover() {
    GET_JSON_VALUE(ndJson, "endpoint", m_sEndpoint, std::string);
    GET_JSON_VALUE(ndJson, "featureSet", m_nFeatureSet, int);
    GET_JSON_VALUE(ndJson, "readOnly", m_bReadOnly, bool);
    GET_JSON_VALUE(ndJson, "modifications", m_sModifications, std::string);
    GET_JSON_VALUE(ndJson, "playerLogin", m_sPlayerLogin, std::string);
    GET_JSON_VALUE(ndJson, "playerPassword", m_sPlayerPassword, std::string);
}

void NodeDatabase::setupJSON() {
    ndJson = nlohmann::json();
}

NodeDatabase::~NodeDatabase() {}

std::vector<std::string> NodeDatabase::getModifications() {
    // wip
    return {m_sModifications};
}