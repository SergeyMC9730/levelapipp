#include "GDServer.h"
#include "curl_backend.h"

#include "Translation.h"

using namespace LevelAPI::Backend;

GDServer::GDServer() {
    m_eStatus = GSS_ONLINE;
}
GDServer::~GDServer() {
    m_eStatus = GSS_OFFLINE;
}
LevelAPI::DatabaseController::Level *GDServer::getLevelMetaByID(int id, bool resolveAccountInfo) {
    return nullptr;
}

std::vector<LevelAPI::DatabaseController::Level *> GDServer::getLevelsBySearchType(int type, std::string str, int page) {
    return {};
}
std::vector<LevelAPI::DatabaseController::Level *> GDServer::getLevelsBySearchType(int type) {
    return getLevelsBySearchType(type, "", 0);
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
    m_sUsername = u;
    m_sPassword = p;
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

std::string GDServer::determineGVFromID(int id) {
    int i = 0;
    while(i < m_vRanges.size()) {
        if((id >= m_vRanges[i]->m_nMin) && (id <= m_vRanges[i]->m_nMax)) {
            return m_vRanges[i]->m_sGDVer;
        }
        i++;
    }
    return "unknown";
}

std::string GDServer::getServerName() {
    return Frontend::Translation::getByKey("lapi.gdserver.name");
}