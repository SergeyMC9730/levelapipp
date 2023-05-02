// stub until changes in api would appear

#include "GDServer_BasementLike21.h"
#include "CURLParameter.h"
#include "GDServer.h"
#include "curl_backend.h"
#include "ThreadSafeLevelParser.h"
#include "lapi_database.h"
#include "Account19.h"
#include "StringSplit.h"
#include "UUID.h"

#include <cstdint>
#include <cstring>
#include <vector>

using namespace LevelAPI::Backend;

GDServer_BasementLike21::GDServer_BasementLike21(std::string *endpoint) : GDServer() {
    m_sEndpointURL = endpoint;

    m_vRanges.push_back(new Tools::LevelRange(1, 103, new std::string("2.1-testing")));
    m_vRanges.push_back(new Tools::LevelRange(104, 9999, new std::string("2.1")));
}
LevelAPI::DatabaseController::Level *GDServer_BasementLike21::getLevelMetaByID(int id, bool resolveAccountInfo) {
    auto m_pLinkedCURL = new CURLConnection();
    
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
    uurl += "/downloadGJLevel22.php";

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

    lvl = LevelParser::parseFromResponse(res->data);
    lvl->m_nRetryAfter = 0;
    
    free((void *)res->data);
    delete res;
    res = nullptr;

    delete m_pLinkedCURL;
    m_pLinkedCURL = nullptr;

    lvl->m_uRelease->m_nGameVersion = lvl->m_nGameVersion;
    delete lvl->m_uRelease->m_fActualVersion;
    lvl->m_uRelease->m_fActualVersion = nullptr;
    lvl->m_uRelease->m_fActualVersion = new std::string(determineGVFromID(lvl->m_nLevelID));

    return lvl;
}

std::vector<LevelAPI::DatabaseController::Level *> GDServer_BasementLike21::getLevelsBySearchType(int type) {
    auto m_pLinkedCURL = new CURLConnection();
    
    m_pLinkedCURL->setDebug(getDebug());

    m_pLinkedCURL->setData({
        new CURLParameter("secret", "Wmfd2893gb7"),
        new CURLParameter("type", type),
        new CURLParameter("page", 0),
        new CURLParameter("gameVersion", getGameVersion())
    });

    std::string uurl = "";
    uurl += *m_sEndpointURL;
    uurl += "/getGJLevels21.php";

    CURLResult *res = m_pLinkedCURL->access_page(uurl.c_str(), "POST");
    if(res->http_status != 200 || res->result != 0) return {};

    std::vector<LevelAPI::DatabaseController::Level *> vec1;
    std::vector<std::string> vec2 = splitString(res->data, '#');

    // parse players
    std::string plList = vec2[1];
    std::string lvlList = vec2[0];
    std::map<int, Account19 *> playerMap;

    std::vector<std::string> vec4array = splitString(plList.c_str(), '|');
    std::vector<std::string> vec5levels = splitString(lvlList.c_str(), '|');
    int i = 0;

    std::vector<Account19 *> accounts;
    
    while(i < vec4array.size()) {
        std::vector<std::string> vec5player = splitString(vec4array[i].c_str(), ':');
        int userID = std::stoi(vec5player[0]);
        std::string username = vec5player[1];
        int accountID = std::stoi(vec5player[2]);
        Account19 *ac20 = new Account19();
        ac20->accountID = accountID;
        ac20->username = username;
        playerMap.insert(std::pair<int, Account19 *>(userID, ac20));
        accounts.push_back(ac20);
        vec5player.clear();
        i++;
    }
    vec4array.clear();
    vec4array = splitString(vec2[1].c_str(), '|');
    i = 0;
    while(i < vec5levels.size()) {
        LevelAPI::DatabaseController::Level *lvl = LevelParser::parseFromResponse(vec5levels[i].c_str());
        Account19 *ac20 = playerMap[lvl->m_nPlayerID];
        lvl->m_nAccountID = ac20->accountID;
        delete lvl->m_sUsername;
        lvl->m_sUsername = nullptr;
        lvl->m_sUsername = new std::string(ac20->username);
        lvl->m_uRelease->m_nGameVersion = lvl->m_nGameVersion;
        delete lvl->m_uRelease->m_fActualVersion;
        lvl->m_uRelease->m_fActualVersion = nullptr;
        lvl->m_uRelease->m_fActualVersion = new std::string(determineGVFromID(lvl->m_nLevelID));
        vec1.push_back(lvl);
        i++;
    }

    i = 0;
    while(i < accounts.size()) {
        delete accounts[i];
        i++;
    }

    accounts.clear();
    vec4array.clear();
    vec5levels.clear();

    free((void *)res->data);
    delete res;
    res = nullptr;

    delete m_pLinkedCURL;
    m_pLinkedCURL = nullptr;

    return vec1;
};
LevelAPI::DatabaseController::Level *GDServer_BasementLike21::resolveLevelData(LevelAPI::DatabaseController::Level *level) {
    auto m_pLinkedCURL = new CURLConnection();
    
    m_pLinkedCURL->setDebug(getDebug());

    m_pLinkedCURL->setData({
        new CURLParameter("secret", "Wmfd2893gb7"),
        new CURLParameter("levelID", level->m_nLevelID),
        new CURLParameter("gameVersion", getGameVersion())
    });

    std::string uurl = "";
    uurl += *m_sEndpointURL;
    uurl += "/downloadGJLevel22.php";

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

    LevelAPI::DatabaseController::Level *lvl = LevelParser::parseFromResponse(res->data);
    delete level->m_sLevelString;
    level->m_sLevelString = nullptr;
    level->m_sLevelString = new std::string(lvl->m_sLevelString->c_str());
    level->m_nMusicID = lvl->m_nMusicID;
    level->m_nSongID = lvl->m_nSongID;
    lvl->m_uRelease->m_nGameVersion = lvl->m_nGameVersion;
    delete level->m_uRelease->m_fActualVersion;
    level->m_uRelease->m_fActualVersion = nullptr;
    level->m_uRelease->m_fActualVersion = new std::string(determineGVFromID(lvl->m_nLevelID));

    free((void *)res->data);
    delete lvl;
    delete res;
    delete m_pLinkedCURL;
    m_pLinkedCURL = nullptr;
    lvl = nullptr;
    res = nullptr;

    return level;
}

GDServerUploadResult *GDServer_BasementLike21::uploadLevel(DatabaseController::Level *level) {
    auto m_pLinkedCURL = new CURLConnection();
    auto res = new GDServerUploadResult();
    
    res->successful = false;
    res->id = 0;
    
    if (level == nullptr) return res;
    if (res == nullptr) return res;
    if (m_sUsername->empty() || m_sPassword->empty()) return res;

    m_pLinkedCURL->setDebug(getDebug());

    m_pLinkedCURL->setData({
        new CURLParameter("secret", "Wmfd2893gb7"),
        new CURLParameter("gameVersion", getGameVersion()),

    });

    delete m_pLinkedCURL;
    m_pLinkedCURL = nullptr;

    return res;
}

int GDServer_BasementLike21::getGameVersion() {
    return 21;
}

bool GDServer_BasementLike21::login() {
    auto m_pLinkedCURL = new CURLConnection();

    m_pLinkedCURL->setDebug(getDebug());

    m_pLinkedCURL->setData({
        new CURLParameter("secret", "Wmfv3899gc9"),
        new CURLParameter("udid", ConnectionCrypt::createUUID()),
        new CURLParameter("password", m_sPassword),
        new CURLParameter("userName", m_sUsername),
        new CURLParameter("gameVersion", getGameVersion())
    });

    std::string uurl = "";
    uurl += *m_sEndpointURL;
    uurl += "/accounts/loginGJAccount.php";

    CURLResult *res = m_pLinkedCURL->access_page(uurl.c_str(), "POST");
    printf("response login: %s\n", res->data);
    printf("%d %d %d\n", res->http_status, res->result, res->retry_after);
    int ra = res->retry_after;
    if(res->http_status != 200 || res->result != 0 || ra != 0) return false;

    delete m_pLinkedCURL;
    m_pLinkedCURL = nullptr;

    return true;
}