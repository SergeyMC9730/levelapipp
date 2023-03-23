#include "GDServer.h"
#include "curl_backend.h"

using namespace LevelAPI::Backend;

GDServer::GDServer() {
    m_pLinkedCURL = new CURLConnection();
}
GDServer::~GDServer() {
    delete m_pLinkedCURL;
    delete m_sEndpointURL;
}
GJGameLevel *GDServer::getLevelMetaByID(int id) {
    return nullptr;
}

std::vector<LevelAPI::DatabaseController::Level *> GDServer::getLevelsBySearch(const char *type) {
    return {};
}

LevelAPI::DatabaseController::Level *GDServer::resolveLevelData(LevelAPI::DatabaseController::Level *level) {
    return nullptr;
}