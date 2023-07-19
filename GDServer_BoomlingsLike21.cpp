#include "GDServer_BoomlingsLike21.h"
#include "Translation.h"

using namespace LevelAPI::Backend;

GDServer_BoomlingsLike21::GDServer_BoomlingsLike21(std::string endpoint) : GDServer_BoomlingsLike19(endpoint) {}

std::string GDServer_BoomlingsLike21::_getDownloadLevelEndpointName() {
    return "downloadGJLevel22.php";
}
std::string GDServer_BoomlingsLike21::_getLevelListEndpointName() {
    return "getGJLevels21.php";
}

int GDServer_BoomlingsLike21::getGameVersion() {
    return 21;
}

std::string GDServer_BoomlingsLike21::getServerName() {
    return Frontend::Translation::getByKey("lapi.gdserver_boomlingslike21.name");
}

std::string GDServer_BoomlingsLike21::getServerIdentifier() {
    return "gdserver_boomlingslike21";
}
