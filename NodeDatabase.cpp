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
    _jsonObject["endpoint"] = m_sEndpoint;
    _jsonObject["featureSet"] = m_nFeatureSet;
    _jsonObject["readOnly"] = m_bReadOnly;
    _jsonObject["playerLogin"] = m_sPlayerLogin;
    _jsonObject["playerPassword"] = m_sPlayerPassword;
    if(!m_sModifications.empty()) _jsonObject["modifications"] = m_sModifications;
}

void NodeDatabase::recover() {
    GET_JSON_VALUE(_jsonObject, "endpoint", m_sEndpoint, std::string);
    GET_JSON_VALUE(_jsonObject, "featureSet", m_nFeatureSet, int);
    GET_JSON_VALUE(_jsonObject, "readOnly", m_bReadOnly, bool);
    GET_JSON_VALUE(_jsonObject, "modifications", m_sModifications, std::string);
    GET_JSON_VALUE(_jsonObject, "playerLogin", m_sPlayerLogin, std::string);
    GET_JSON_VALUE(_jsonObject, "playerPassword", m_sPlayerPassword, std::string);
}

void NodeDatabase::setupJSON() {
    _jsonObject = nlohmann::json();
}

NodeDatabase::~NodeDatabase() {}

std::vector<std::string> NodeDatabase::getModifications() {
    // wip
    return {m_sModifications};
}