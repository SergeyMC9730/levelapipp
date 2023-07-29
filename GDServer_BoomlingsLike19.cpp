#include "GDServer_BoomlingsLike19.h"

#include "CURLParameter.h"
#include "UUID.h"
#include "curl_backend.h"
#include "ThreadSafeLevelParser.h"
#include "Account19.h"
#include "StringSplit.h"
#include "Translation.h"
#include "UUID.h"

#include <cstdint>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace LevelAPI::Backend;

GDServer_BoomlingsLike19::GDServer_BoomlingsLike19(std::string endpoint) : GDServer() {
    m_sEndpointURL = endpoint;
}

std::string GDServer_BoomlingsLike19::_getLoginAccountEndpointName() {
    return "loginGJAccount.php";
}
std::string GDServer_BoomlingsLike19::_getDownloadLevelEndpointName() {
    return "downloadGJLevel19.php";
}
std::string GDServer_BoomlingsLike19::_getLevelListEndpointName() {
    return "getGJLevels19.php";
}

int GDServer_BoomlingsLike19::getMaxLevelPageSize() {
    return 10;
}
int GDServer_BoomlingsLike19::getMaxMapPackPageSize() {
    return 5;
};

bool GDServer_BoomlingsLike19::login() {
    auto m_pLinkedCURL = new CURLConnection();

    m_pLinkedCURL->setDebug(getDebug());

    m_pLinkedCURL->setData({
        new CURLParameter("secret", _getSecretValueExtra()),
        new CURLParameter("udid", ConnectionCrypt::createUUID()),
        new CURLParameter("password", m_sPassword),
        new CURLParameter("userName", m_sUsername),
        new CURLParameter("gameVersion", getGameVersion())
    });

    std::string uurl = m_sEndpointURL + "/accounts/" + _getLoginAccountEndpointName();

    CURLResult *res = m_pLinkedCURL->access_page(uurl.c_str(), "POST");
    printf("response login: %s\n", res->data);
    printf("%d %d %d\n", res->http_status, res->result, res->retry_after);
    int ra = res->retry_after;
    if(res->http_status != 200 || res->result != 0) {
        std::string ddd = res->data;
        std::vector<std::string> pban_responses {
            "error code: 1005",
            "error code: 1006",
            "error code: 1007",
            "error code: 1008",
            "error code: 1009",
            "error code: 1012",
            "error code: 1106"
        };
        int q = 0;
        while(q < pban_responses.size()) {
            if(ddd.find(pban_responses[q]) != std::string::npos) {
                m_eStatus = GSS_PERMANENT_BAN;
            }
            q++;
        }
        delete res;
        delete m_pLinkedCURL;
        m_pLinkedCURL = nullptr;
        res = nullptr;
        return false;
    }

    delete m_pLinkedCURL;
    m_pLinkedCURL = nullptr;

    return true;
}

LevelAPI::DatabaseController::Level *GDServer_BoomlingsLike19::getLevelMetaByID(int id, bool resolveAccountInfo, std::optional<CurlProxy> proxy) {
    auto m_pLinkedCURL = new CURLConnection();

    if (proxy.has_value()) {
        m_pLinkedCURL->setProxy(proxy.value());
    }
     
    LevelAPI::DatabaseController::Level *lvl = nullptr;
    
    if (id <= 0) {
        lvl = new DatabaseController::Level("");
        lvl->m_nRetryAfter = id - 1;
        delete m_pLinkedCURL;
        m_pLinkedCURL = nullptr;
        return lvl;
    }

    m_pLinkedCURL->setDebug(getDebug());

    m_pLinkedCURL->setData({
        new CURLParameter("secret", _getSecretValueStandard()),
        new CURLParameter("levelID", id),
        new CURLParameter("gameVersion", getGameVersion())
    });

    std::string uurl = m_sEndpointURL + "/" + _getDownloadLevelEndpointName();

    CURLResult *res = m_pLinkedCURL->access_page(uurl.c_str(), "POST");
    //printf("response 2: %s\n", res->data);
    // level->m_nRetryAfter = res->retry_after;

    if(res->http_status != 200 || res->result != 0) {
        printf("returned 2 %d %d %d %s\n", res->http_status, res->result, res->retry_after, res->data);
        std::string ddd = res->data;
        std::vector<std::string> pban_responses {
            "error code: 1005",
            "error code: 1006",
            "error code: 1007",
            "error code: 1008",
            "error code: 1009",
            "error code: 1012",
            "error code: 1106"
        };
        int q = 0;
        while(q < pban_responses.size()) {
            if(ddd.find(pban_responses[q]) != std::string::npos) {
                m_eStatus = GSS_PERMANENT_BAN;
            }
            q++;
        }
        delete res;
        delete m_pLinkedCURL;
        m_pLinkedCURL = nullptr;
        res = nullptr;
        return lvl;
    }

    std::string strtest = res->data;
    if(!strtest.compare("-1")) {
        delete res;
        delete m_pLinkedCURL;
        res = nullptr;
        m_pLinkedCURL = nullptr;

        lvl = new DatabaseController::Level("");
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
    lvl->m_uRelease->m_fActualVersion = determineGVFromID(lvl->m_nLevelID);

    return lvl;
}

std::vector<LevelAPI::DatabaseController::Level *> GDServer_BoomlingsLike19::getLevelsBySearchType(int type, std::string str, int page) {
    auto m_pLinkedCURL = new CURLConnection();
    
    m_pLinkedCURL->setDebug(getDebug());

    m_pLinkedCURL->setData({
        new CURLParameter("secret", _getSecretValueStandard()),
        new CURLParameter("type", type),
        new CURLParameter("page", page),
        new CURLParameter("gameVersion", getGameVersion()),
        new CURLParameter("str", str)
    });

    std::string uurl = m_sEndpointURL + "/" + _getLevelListEndpointName();

    CURLResult *res = m_pLinkedCURL->access_page(uurl.c_str(), "POST");
    if(res->http_status != 200 || res->result != 0) {
        std::string ddd = res->data;
        std::vector<std::string> pban_responses {
            "error code: 1005",
            "error code: 1006",
            "error code: 1007",
            "error code: 1008",
            "error code: 1009",
            "error code: 1012",
            "error code: 1106"
        };
        int q = 0;
        while(q < pban_responses.size()) {
            if(ddd.find(pban_responses[q]) != std::string::npos) {
                m_eStatus = GSS_PERMANENT_BAN;
            }
            q++;
        }
        delete res;
        delete m_pLinkedCURL;
        m_pLinkedCURL = nullptr;
        res = nullptr;
        return {};
    }
    if(res->data[0] == '-') {
        delete res;
        delete m_pLinkedCURL;
        res = nullptr;
        m_pLinkedCURL = nullptr;
        return {};
    }

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
        lvl->m_sUsername = ac20->username;
        lvl->m_uRelease->m_nGameVersion = lvl->m_nGameVersion;
        lvl->m_uRelease->m_fActualVersion = determineGVFromID(lvl->m_nLevelID);
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

    std::reverse(vec1.begin(), vec1.end());

    return vec1;
};

std::string GDServer_BoomlingsLike19::_getSecretValueStandard() {
    return "Wmfd2893gb7";
}
std::string GDServer_BoomlingsLike19::_getSecretValueExtra() {
    return "Wmfv3899gc9";
}

LevelAPI::DatabaseController::Level *GDServer_BoomlingsLike19::resolveLevelData(LevelAPI::DatabaseController::Level *level, std::optional<CurlProxy> proxy) {
    auto m_pLinkedCURL = new CURLConnection();
    
    m_pLinkedCURL->setDebug(getDebug());

    if (proxy.has_value()) {
        m_pLinkedCURL->setProxy(proxy.value());
    }

    m_pLinkedCURL->setData({
        new CURLParameter("secret", _getSecretValueStandard()),
        new CURLParameter("levelID", level->m_nLevelID),
        new CURLParameter("gameVersion", getGameVersion())
    });

    std::string uurl = m_sEndpointURL + "/" +_getDownloadLevelEndpointName();

    CURLResult *res = m_pLinkedCURL->access_page(uurl.c_str(), "POST");
    //printf("response 2: %s\n", res->data);
    level->m_nRetryAfter = res->retry_after;
    if(res->http_status != 200 || res->result != 0) {
        std::string ddd = res->data;
        std::vector<std::string> pban_responses {
            "error code: 1005",
            "error code: 1006",
            "error code: 1007",
            "error code: 1008",
            "error code: 1009",
            "error code: 1012",
            "error code: 1106"
        };
        int q = 0;
        while(q < pban_responses.size()) {
            if(ddd.find(pban_responses[q]) != std::string::npos) {
                m_eStatus = GSS_PERMANENT_BAN;
            }
            q++;
        }
        delete res;
        delete m_pLinkedCURL;
        res = nullptr;
        m_pLinkedCURL = nullptr;
        return level;
    }

    std::string strtest = res->data;
    if(!strtest.compare("-1")) {
        delete res;
        res = nullptr;

        level->m_nRetryAfter = -128;
        return level;
    }

    LevelAPI::DatabaseController::Level *lvl = LevelParser::parseFromResponse(res->data);
    level->m_sLevelString = lvl->m_sLevelString;
    level->m_nMusicID = lvl->m_nMusicID;
    level->m_nSongID = lvl->m_nSongID;
    level->m_uRelease->m_nGameVersion = lvl->m_nGameVersion;
    level->m_uRelease->m_fActualVersion = determineGVFromID(lvl->m_nLevelID);

    free((void *)res->data);
    delete lvl;
    delete res;
    delete m_pLinkedCURL;
    m_pLinkedCURL = nullptr;
    lvl = nullptr;
    res = nullptr;

    return level;
}

int GDServer_BoomlingsLike19::getGameVersion() {
    return 19;
}
std::string GDServer_BoomlingsLike19::getServerName() {
    return Frontend::Translation::getByKey("lapi.gdserver_boomlingslike19.name");
}
std::string GDServer_BoomlingsLike19::getServerIdentifier() {
    return "gdserver_boomlingslike19";
}

GDServerUploadResult *GDServer_BoomlingsLike19::uploadLevel(DatabaseController::Level *level) {
    auto m_pLinkedCURL = new CURLConnection();
    auto res = new GDServerUploadResult();
    
    res->successful = false;
    res->id = 0;
    
    if (level == nullptr) {
        delete m_pLinkedCURL;
        m_pLinkedCURL = nullptr;
        return res;
    }
    if (res == nullptr) {
        delete m_pLinkedCURL;
        m_pLinkedCURL = nullptr;
        return res;
    }
    if (m_sUsername.empty() || m_sPassword.empty()) {
        delete m_pLinkedCURL;
        m_pLinkedCURL = nullptr;
        return res;
    }

    m_pLinkedCURL->setDebug(getDebug());

    m_pLinkedCURL->setData({
        new CURLParameter("secret", _getSecretValueStandard()),
        new CURLParameter("gameVersion", getGameVersion())

    });

    delete m_pLinkedCURL;
    m_pLinkedCURL = nullptr;

    return res;
}