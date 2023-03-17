#include "GDServer.h"
#include "curl_backend.h"

using namespace LevelAPI::Backend;

GDServer::GDServer() {
    m_pLinkedCURL = new CURLConnection();
}
GDServer::~GDServer() {
    m_pLinkedCURL->destroy();
}
GJGameLevel *GDServer::getLevelMetaByID(int id) {
    return nullptr;
}