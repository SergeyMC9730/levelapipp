#include "GDServer_19GDPS.h"
#include <string>
#include "Translation.h"

LevelAPI::Backend::GDServer_19GDPS::GDServer_19GDPS(std::string endpoint) : GDServer_BoomlingsLike19(endpoint) {
    m_vRanges.push_back(new LevelAPI::LevelRange(0, 0x0FFFFFFF, "1.9"));
}

std::string LevelAPI::Backend::GDServer_19GDPS::getServerName() {
    return Frontend::Translation::getByKey("lapi.gdserver_19gdps.name");
}