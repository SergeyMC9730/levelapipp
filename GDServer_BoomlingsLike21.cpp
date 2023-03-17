#include "GDServer_BoomlingsLike21.h"
#include "GDServer.h"
#include "gmd2pp/level-converter/GJGameLevel.h"

using namespace LevelAPI::Backend;

GDServer_BoomlingsLike21::GDServer_BoomlingsLike21(std::string endpoint) : GDServer() {
    m_sEndpointURL = endpoint;
}
GDServer_BoomlingsLike21::~GDServer_BoomlingsLike21() {
    GDServer::~GDServer();
}
GJGameLevel *GDServer_BoomlingsLike21::getLevelMetaByID(int id) {
    return nullptr;
}