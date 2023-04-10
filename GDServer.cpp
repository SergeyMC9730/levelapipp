#include "GDServer.h"
#include "curl_backend.h"

using namespace LevelAPI::Backend;

GDServer::GDServer() {
    m_pLinkedCURL = new CURLConnection();

    m_sUsername = new std::string("");
    m_sPassword = new std::string("");
}
GDServer::~GDServer() {
    delete m_pLinkedCURL;
    delete m_sEndpointURL;
    delete m_sUsername;
    delete m_sPassword;

    m_pLinkedCURL = nullptr;
    m_sEndpointURL = nullptr;
    m_sUsername = nullptr;
    m_sPassword = nullptr;
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

void GDServer::setCredintials(std::string u, std::string p) {
    delete m_sUsername;
    delete m_sPassword;

    m_sUsername = nullptr;
    m_sPassword = nullptr;

    m_sUsername = new std::string(u);
    m_sPassword = new std::string(p);
}

GDServerUploadResult *GDServer::uploadLevel(DatabaseController::Level *level) {
    return nullptr;
}

bool GDServer::login() {
    return false;
}

int GDServer::getGameVersion() {
    return 0;
}