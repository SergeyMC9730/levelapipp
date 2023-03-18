#include "GDServer.h"
#include "curl_backend.h"

using namespace LevelAPI::Backend;

GDServer::GDServer() {
    m_pLinkedCURL = new CURLConnection();
}
GDServer::~GDServer() {
    printf("~gdserver\n");
    m_pLinkedCURL->destroy();
    delete m_pLinkedCURL;
    delete m_sEndpointURL;
}
GJGameLevel *GDServer::getLevelMetaByID(int id) {
    return nullptr;
}

std::vector<LevelAPI::DatabaseController::Level *> GDServer::getLevelsBySearch(const char *type) {
    return {};
}