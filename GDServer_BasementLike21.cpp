// stub until changes in api would appear

#include "GDServer_BasementLike21.h"
#include "CURLParameter.h"
#include "GDServer.h"
#include "curl_backend.h"
#include "ThreadSafeLevelParser.h"
#include "lapi_database.h"
#include "Account21.h"
#include "StringSplit.h"
#include "UUID.h"
#include "Translation.h"

#include <cstdint>
#include <cstring>
#include <vector>

using namespace LevelAPI::Backend;

GDServer_BasementLike21::GDServer_BasementLike21(std::string endpoint) : GDServer() {
    m_sEndpointURL = endpoint;
}
LevelAPI::DatabaseController::Level *GDServer_BasementLike21::getLevelMetaByID(int id, bool resolveAccountInfo) {
    auto m_pLinkedCURL = new CURLConnection();
    
    LevelAPI::DatabaseController::Level *lvl;
    
    if (id <= 0) {
        lvl = new DatabaseController::Level("");
        lvl->m_nRetryAfter = id - 1;
        delete m_pLinkedCURL;
        m_pLinkedCURL = nullptr;
        return lvl;
    }

    m_pLinkedCURL->setDebug(getDebug());

    m_pLinkedCURL->setData({
        new CURLParameter("secret", "Wmfd2893gb7"),
        new CURLParameter("levelID", id),
        new CURLParameter("gameVersion", getGameVersion())
    });

    std::string uurl = m_sEndpointURL + "/downloadGJLevel22.php";

    CURLResult *res = m_pLinkedCURL->access_page(uurl.c_str(), "POST");
    //printf("response 2: %s\n", res->data);
    // level->m_nRetryAfter = res->retry_after;

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
        delete m_pLinkedCURL;
        delete res;
        m_pLinkedCURL = nullptr;
        res = nullptr;
        return lvl;
    }

    std::string strtest = res->data;
    if(!strtest.compare("-1")) {
        delete res;
        delete m_pLinkedCURL;
        m_pLinkedCURL = nullptr;
        res = nullptr;

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

std::vector<LevelAPI::DatabaseController::Level *> GDServer_BasementLike21::getLevelsBySearchType(int type, std::string str, int page) {
    auto m_pLinkedCURL = new CURLConnection();
    
    m_pLinkedCURL->setDebug(getDebug());

    m_pLinkedCURL->setData({
        new CURLParameter("secret", "Wmfd2893gb7"),
        new CURLParameter("type", type),
        new CURLParameter("page", page),
        new CURLParameter("gameVersion", getGameVersion()),
        new CURLParameter("str", str)
    });

    std::string uurl = m_sEndpointURL + "/getGJLevels21.php";

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
        m_pLinkedCURL = nullptr;
        res = nullptr;
        return {};
    }

    std::vector<LevelAPI::DatabaseController::Level *> vec1;
    std::vector<std::string> vec2 = splitString(res->data, '#');

    // parse players
    std::string plList = vec2[1];
    std::string lvlList = vec2[0];
    std::map<int, Account21 *> playerMap;

    std::vector<std::string> vec4array = splitString(plList.c_str(), '|');
    std::vector<std::string> vec5levels = splitString(lvlList.c_str(), '|');
    int i = 0;

    std::vector<Account21 *> accounts;
    
    while(i < vec4array.size()) {
        std::vector<std::string> vec5player = splitString(vec4array[i].c_str(), ':');
        int userID = std::stoi(vec5player[0]);
        std::string username = vec5player[1];
        int accountID = std::stoi(vec5player[2]);
        Account21 *ac20 = new Account21();
        ac20->accountID = accountID;
        ac20->username = username;
        playerMap.insert(std::pair<int, Account21 *>(userID, ac20));
        accounts.push_back(ac20);
        vec5player.clear();
        i++;
    }
    vec4array.clear();
    vec4array = splitString(vec2[1].c_str(), '|');
    i = 0;
    while(i < vec5levels.size()) {
        LevelAPI::DatabaseController::Level *lvl = LevelParser::parseFromResponse(vec5levels[i].c_str());
        Account21 *ac20 = playerMap[lvl->m_nPlayerID];
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
LevelAPI::DatabaseController::Level *GDServer_BasementLike21::resolveLevelData(LevelAPI::DatabaseController::Level *level) {
    auto m_pLinkedCURL = new CURLConnection();
    
    m_pLinkedCURL->setDebug(getDebug());

    m_pLinkedCURL->setData({
        new CURLParameter("secret", "Wmfd2893gb7"),
        new CURLParameter("levelID", level->m_nLevelID),
        new CURLParameter("gameVersion", getGameVersion())
    });

    std::string uurl = m_sEndpointURL + "/downloadGJLevel22.php";

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
        m_pLinkedCURL = nullptr;
        res = nullptr;
        return level;
    }

    std::string strtest = res->data;
    if(!strtest.compare("-1")) {
        delete res;
        delete m_pLinkedCURL;
        m_pLinkedCURL = nullptr;
        res = nullptr;

        level->m_nRetryAfter = -128;
        return level;
    }

    LevelAPI::DatabaseController::Level *lvl = LevelParser::parseFromResponse(res->data);
    level->m_sLevelString = lvl->m_sLevelString;
    level->m_nMusicID = lvl->m_nMusicID;
    level->m_nSongID = lvl->m_nSongID;
    lvl->m_uRelease->m_nGameVersion = lvl->m_nGameVersion;
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

GDServerUploadResult *GDServer_BasementLike21::uploadLevel(DatabaseController::Level *level) {
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

    std::string uurl = m_sEndpointURL + "/accounts/loginGJAccount.php";

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

std::string GDServer_BasementLike21::getServerName() {
    return Frontend::Translation::getByKey("lapi.gdserver_basementlike21.name");
}
std::string GDServer_BasementLike21::getServerIdentifier() {
    return "gdserver_basementlike21";
}