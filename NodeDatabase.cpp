#include "lapi_database.h"

using namespace LevelAPI::DatabaseController;

NodeDatabase::NodeDatabase(std::string endpoint, uint8_t featureSet, bool readOnly) {
    m_sEndpoint = endpoint;
    m_nFeatureSet = featureSet;
    m_bReadOnly = readOnly;
}
NodeDatabase::NodeDatabase() {
    NodeDatabase("", 21, true);
}