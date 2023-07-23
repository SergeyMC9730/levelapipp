#include "GDServer_BasementGDPS.h"
#include "Translation.h"

LevelAPI::Backend::GDServer_BasementGDPS::GDServer_BasementGDPS(std::string endpoint) : GDServer_BasementLike21(endpoint) {
    m_vRanges.push_back(new LevelAPI::LevelRange(1, 9999, "2.1"));
}

std::string LevelAPI::Backend::GDServer_BasementGDPS::getServerName() {
    return Frontend::Translation::getByKey("lapi.gdserver_basementgdps.name");
}
std::string LevelAPI::Backend::GDServer_BasementGDPS::getServerIdentifier() {
    return "gdserver_basementGDPS";
}