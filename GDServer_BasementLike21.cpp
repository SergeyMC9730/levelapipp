// stub until changes in api would appear

#include "GDServer_BasementLike21.h"
#include "Translation.h"

using namespace LevelAPI::Backend;

GDServer_BasementLike21::GDServer_BasementLike21(std::string endpoint) : GDServer_BoomlingsLike21(endpoint) {}

std::string GDServer_BasementLike21::getServerName() {
    return Frontend::Translation::getByKey("lapi.gdserver_basementlike21.name");
}
std::string GDServer_BasementLike21::getServerIdentifier() {
    return "gdserver_basementlike21";
}