#include "GDServer_BoomlingsLike19.h"

#include "GDServer.h"
#include "curl_backend.h"
#include "ThreadSafeLevelParser.h"
#include "lapi_database.h"
#include "Account20.h"
#include "StringSplit.h"

#include <cstdint>
#include <cstring>
#include <vector>

using namespace LevelAPI::Backend;

GDServer_BoomlingsLike19::GDServer_BoomlingsLike19(std::string *endpoint) : GDServer() {
    m_sEndpointURL = endpoint;
}
LevelAPI::DatabaseController::Level *GDServer_BoomlingsLike19::getLevelMetaByID(int id, bool resolveAccountInfo) {
    LevelAPI::DatabaseController::Level *lvl;
    
    if (id <= 0) {
        lvl = new DatabaseController::Level();
        lvl->m_nRetryAfter = id - 1;
        return lvl;
    }

    m_pLinkedCURL->setDebug(getDebug());

    m_pLinkedCURL->setData({
        new CURLParameter("secret", "Wmfd2893gb7"),
        new CURLParameter("levelID", id),
        new CURLParameter("gameVersion", getGameVersion())
    });

    std::string uurl = "";
    uurl += *m_sEndpointURL;
    uurl += "/downloadGJLevel19.php";

    CURLResult *res = m_pLinkedCURL->access_page(uurl.c_str(), "POST");
    //printf("response 2: %s\n", res->data);
    // level->m_nRetryAfter = res->retry_after;

    if(res->http_status != 200 || res->result != 0) {
        lvl = new DatabaseController::Level();
        lvl->m_nRetryAfter = res->retry_after;
        delete res;
        res = nullptr;
        return lvl;
    }

    std::string strtest = "";
    strtest += res->data;
    if(!strtest.compare("-1")) {
        delete res;
        res = nullptr;

        lvl = new DatabaseController::Level();
        lvl->m_nRetryAfter = -128;
        return lvl;
    }

    lvl = LevelParser::parseFromResponse(res->data, true);
    lvl->m_nRetryAfter = 0;
    
    free((void *)res->data);
    delete res;
    res = nullptr;

    return lvl;
}

std::vector<LevelAPI::DatabaseController::Level *> GDServer_BoomlingsLike19::getLevelsBySearchType(int type) {
    m_pLinkedCURL->setDebug(getDebug());

    m_pLinkedCURL->setData({
        new CURLParameter("secret", "Wmfd2893gb7"),
        new CURLParameter("type", type),
        new CURLParameter("gameVersion", getGameVersion())
    });

    std::string uurl = "";
    uurl += *m_sEndpointURL;
    uurl += "/getGJLevels19.php";

    CURLResult *res = m_pLinkedCURL->access_page(uurl.c_str(), "POST");
    if(res->http_status != 200 || res->result != 0) return {};

    std::vector<LevelAPI::DatabaseController::Level *> vec1;
    std::vector<std::string> vec2 = splitString(res->data, '#');

    // parse players
    std::string plList = vec2[1];
    std::string lvlList = vec2[0];
    std::map<int, Account20> playerMap;

    std::vector<std::string> vec4array = splitString(plList.c_str(), '|');
    std::vector<std::string> vec5levels = splitString(lvlList.c_str(), '|');
    int i = 0;
    
    while(i < vec4array.size()) {
        std::vector<std::string> vec5player = splitString(vec4array[i].c_str(), ':');
        int userID = std::stoi(vec5player[0]);
        std::string username = vec5player[1];
        int accountID = std::stoi(vec5player[2]);
        Account20 ac20 = Account20();
        ac20.accountID = accountID;
        ac20.username = username;
        playerMap.insert(std::pair<int, Account20>(userID, ac20));
        vec5player.clear();
        i++;
    }
    vec4array.clear();
    vec4array = splitString(vec2[1].c_str(), '|');
    i = 0;
    while(i < vec5levels.size()) {
        LevelAPI::DatabaseController::Level *lvl = LevelParser::parseFromResponse(vec5levels[i].c_str(), true);
        Account20 ac20 = playerMap[lvl->m_nPlayerID];
        lvl->m_nAccountID = ac20.accountID;
        delete lvl->m_sUsername;
        lvl->m_sUsername = nullptr;
        lvl->m_sUsername = new std::string(ac20.username);
        vec1.push_back(lvl);
        i++;
    }

    vec4array.clear();
    vec5levels.clear();

    free((void *)res->data);
    delete res;
    res = nullptr;

    return vec1;
};
LevelAPI::DatabaseController::Level *GDServer_BoomlingsLike19::resolveLevelData(LevelAPI::DatabaseController::Level *level) {
    m_pLinkedCURL->setDebug(getDebug());

    m_pLinkedCURL->setData({
        new CURLParameter("secret", "Wmfd2893gb7"),
        new CURLParameter("levelID", level->m_nLevelID),
        new CURLParameter("gameVersion", getGameVersion())
    });

    std::string uurl = "";
    uurl += *m_sEndpointURL;
    uurl += "/downloadGJLevel19.php";

    CURLResult *res = m_pLinkedCURL->access_page(uurl.c_str(), "POST");
    //printf("response 2: %s\n", res->data);
    level->m_nRetryAfter = res->retry_after;
    if(res->http_status != 200 || res->result != 0) return level;

    std::string strtest = "";
    strtest += res->data;
    if(!strtest.compare("-1")) {
        delete res;
        res = nullptr;

        level->m_nRetryAfter = -128;
        return level;
    }

    LevelAPI::DatabaseController::Level *lvl = LevelParser::parseFromResponse(res->data, true);
    delete level->m_sLevelString;
    level->m_sLevelString = nullptr;
    level->m_sLevelString = new std::string(lvl->m_sLevelString->c_str());
    level->m_nMusicID = lvl->m_nMusicID;
    level->m_nSongID = lvl->m_nSongID;

    free((void *)res->data);
    delete lvl;
    delete res;
    lvl = nullptr;
    res = nullptr;

    return level;
}

int GDServer_BoomlingsLike19::getGameVersion() {
    return 19;
}