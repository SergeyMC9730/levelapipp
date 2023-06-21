#include "GDServer_BoomlingsExperimental22.h"
#include "Translation.h"

LevelAPI::Backend::GDServer_BoomlingsExperimental22::GDServer_BoomlingsExperimental22(std::string endpoint) : GDServer_BoomlingsLike22(endpoint) {
    m_vRanges.push_back(new LevelAPI::LevelRange(91, 1941, "1.0"));
    m_vRanges.push_back(new LevelAPI::LevelRange(1942, 10043, "1.1"));
    m_vRanges.push_back(new LevelAPI::LevelRange(10044, 63415, "1.2"));
    m_vRanges.push_back(new LevelAPI::LevelRange(63416, 121068, "1.3"));
    m_vRanges.push_back(new LevelAPI::LevelRange(121069, 184425, "1.4"));
    m_vRanges.push_back(new LevelAPI::LevelRange(184426, 420780, "1.5"));
    m_vRanges.push_back(new LevelAPI::LevelRange(420781, 827308, "1.6"));
    m_vRanges.push_back(new LevelAPI::LevelRange(827309, 1627362, "1.7"));
    m_vRanges.push_back(new LevelAPI::LevelRange(1627363, 2810918, "1.8"));
    m_vRanges.push_back(new LevelAPI::LevelRange(2810919, 11020426, "1.9"));
    m_vRanges.push_back(new LevelAPI::LevelRange(11020427, 28356225, "2.0"));
    m_vRanges.push_back(new LevelAPI::LevelRange(28356226, 90900002, "2.1"));
    m_vRanges.push_back(new LevelAPI::LevelRange(99000002, 100800002, "2.2"));
}

std::string LevelAPI::Backend::GDServer_BoomlingsExperimental22::getServerName() {
    return Frontend::Translation::getByKey("lapi.gdserver_boomlingsexperimental22.name");
}

std::string LevelAPI::Backend::GDServer_BoomlingsExperimental22::getServerIdentifier() {
    return "GDServer_BoomlingsExperimental22";
}