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
LevelAPI::DatabaseController::Level *GDServer::getLevelMetaByID(int id, bool resolveAccountInfo) {
    return nullptr;
}

std::vector<LevelAPI::DatabaseController::Level *> GDServer::getLevelsBySearchType(int type) {
    return {};
}

LevelAPI::DatabaseController::Level *GDServer::resolveLevelData(LevelAPI::DatabaseController::Level *level) {
    return nullptr;
}

void GDServer::setDebug(bool d) {
	m_bDebug = d;
}
bool GDServer::getDebug() {
	return m_bDebug;
}
