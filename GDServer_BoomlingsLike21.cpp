#include "GDServer_BoomlingsLike21.h"
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

GDServer_BoomlingsLike21::GDServer_BoomlingsLike21(std::string *endpoint) : GDServer() {
    m_sEndpointURL = endpoint;
}
GJGameLevel *GDServer_BoomlingsLike21::getLevelMetaByID(int id) {
    return nullptr;
}

std::vector<LevelAPI::DatabaseController::Level *> GDServer_BoomlingsLike21::getLevelsBySearchType(int type) {
    m_pLinkedCURL->setData({
        new CURLParameter("secret", "Wmfd2893gb7"),
        new CURLParameter("type", type)
    });
    CURLResult *res = m_pLinkedCURL->access_page("https://www.boomlings.com/database/getGJLevels21.php", "POST");
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
        LevelAPI::DatabaseController::Level *lvl = LevelParser::parseFromResponse(vec5levels[i].c_str());
        Account20 ac20 = playerMap[lvl->m_nPlayerID];
        lvl->m_nAccountID = ac20.accountID;
        lvl->m_sUsername = new std::string(ac20.username);
        vec1.push_back(lvl);
        i++;
    }

    vec4array.clear();
    vec5levels.clear();
    return vec1;
};
LevelAPI::DatabaseController::Level *GDServer_BoomlingsLike21::resolveLevelData(LevelAPI::DatabaseController::Level *level) {
    // m_pLinkedCURL->setData(std::vector<CURLParameter *> parameters)
    m_pLinkedCURL->setData({
        new CURLParameter("secret", "Wmfd2893gb7"),
        new CURLParameter("type", 0)
    });

    return nullptr;
}