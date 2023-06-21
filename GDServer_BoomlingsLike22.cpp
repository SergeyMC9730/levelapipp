#include "GDServer_BoomlingsLike22.h"
#include "CURLParameter.h"
#include "GDServer.h"
#include "curl_backend.h"
#include "ThreadSafeLevelParser.h"
#include "lapi_database.h"
#include "Account21.h"
#include "StringSplit.h"
#include "UUID.h"
#include "Translation.h"

#include "TestingBoomlings22.h"

#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>

using namespace LevelAPI::Backend;

GDServer_BoomlingsLike22::GDServer_BoomlingsLike22(std::string endpoint) : GDServer_BoomlingsLike21(endpoint) {}

int GDServer_BoomlingsLike22::getGameVersion() {
    return 22;
}

std::vector<LevelAPI::DatabaseController::Level *> GDServer_BoomlingsLike22::fetchListOfLevels(std::vector<int> levels) {
    if (levels.size() > 100) return {nullptr};

    std::string str = "";

    int size = levels.size();

    int i = 0;
    while(i < size) {
        str += std::to_string(levels[i]);

        if (i + 1 != size) {
            str += ",";
        }

        i++;
    }
    
    return getLevelsBySearchType(26, str, 0);
}

std::string GDServer_BoomlingsLike22::getServerName() {
    return Frontend::Translation::getByKey("lapi.gdserver_boomlingslike22.name");
}

std::string GDServer_BoomlingsLike22::getServerIdentifier() {
    return "GDServer_BoomlingsLike22";
}

std::vector<LevelAPI::DatabaseController::Level *> GDServer_BoomlingsLike22::getReportedLevels() {
    return {}; // cannot test though
}