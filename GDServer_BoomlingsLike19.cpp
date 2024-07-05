/**
 *  LevelAPI - Geometry Dash level cacher with search functionality and more.
    Copyright (C) 2024  Sergei Baigerov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "GDServer_BoomlingsLike19.h"
// #include "RobTopStringContainer.hpp"
#include "LevelRange.h"
#include "UUID.h"
#include "curl_backend.h"
#include "ThreadSafeLevelParser.h"
#include "Account19.h"
#include "StringSplit.h"
#include "Translation.h"
#include "UUID.h"
#include "Level.h"
#include "LevelRelease.h"

#include <cstdint>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace LevelAPI::Backend;

GDServer_BoomlingsLike19::GDServer_BoomlingsLike19(std::string endpoint) : GDServer() {
    m_sEndpointURL = endpoint;
}
GDServer_BoomlingsLike19::GDServer_BoomlingsLike19(std::string endpoint, std::vector<LevelRange> list) : GDServer(list) {
    m_sEndpointURL = endpoint;
}
GDServer_BoomlingsLike19::GDServer_BoomlingsLike19(std::string endpoint, LevelRangeList list) : GDServer(list) {
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

bool GDServer_BoomlingsLike19::login(std::optional<CurlProxy> proxy) {
    // create curl instance
    auto m_pLinkedCURL = _setupCURL(proxy, _getSecretValueExtra());

    // add parameters
    m_pLinkedCURL->addData({
        new CURLParameter("udid", ConnectionCrypt::createUUID()),
        new CURLParameter("password", m_sPassword),
        new CURLParameter("username", m_sUsername)
    });
    // set url
    std::string url = m_sEndpointURL + "/accounts/" + _getLoginAccountEndpointName();
    // create request to the server
    CURLResult *res = m_pLinkedCURL->access_page(url.c_str(), "POST");
    
    printf("response login: %s\n", res->data);
    printf("%d %d %d\n", res->http_status, res->result, res->retry_after);

    if (!processCURLAnswer(res)) {
        delete m_pLinkedCURL; m_pLinkedCURL = nullptr;

        free((char *)res->data);
        delete res; res = nullptr;

        return false;
    } 

    delete m_pLinkedCURL; m_pLinkedCURL = nullptr;
    delete res; res = nullptr;

    return true;
}

LevelAPI::DatabaseController::Level *GDServer_BoomlingsLike19::downloadLevel(int id, std::optional<CurlProxy> proxy) {
    // create curl instance
    auto m_pLinkedCURL = _setupCURL(proxy, _getSecretValueStandard());

    // add parameters
    m_pLinkedCURL->addData({
        new CURLParameter("levelID", id)
    });

    // create url
    std::string url = m_sEndpointURL + "/" + _getDownloadLevelEndpointName();
    // send request to the server
    CURLResult *res = m_pLinkedCURL->access_page(url.c_str(), "POST");

    if (!processCURLAnswer(res)) {
        free((char *)res->data);
        delete res; res = nullptr;
        
        delete m_pLinkedCURL; m_pLinkedCURL = nullptr;

        return nullptr;
    }

    // check if level doesn't exist
    std::string strtest = res->data;
    if(!strtest.compare("-1")) {
        free((char *)res->data);
        delete res; res = nullptr;
        
        delete m_pLinkedCURL; m_pLinkedCURL = nullptr;

        return nullptr;
    }

    // parse level
    auto lvl = LevelParser::parseFromResponse(res->data);

    // set values
    lvl->m_uRelease->m_nGameVersion = lvl->m_nGameVersion;
    lvl->m_uRelease->m_fActualVersion = determineGVFromID(lvl->m_nLevelID);

    free((char *)res->data);
    delete res; res = nullptr;
        
    delete m_pLinkedCURL; m_pLinkedCURL = nullptr;

    return lvl;
}

std::vector<CURLParameter *> GDServer_BoomlingsLike19::_setupGJLevelsArgs(int type, std::string str, int page) {
    std::string _str = str;
    if (str.empty()) _str = "-";
    
    return {
        new CURLParameter("type", type),
        new CURLParameter("page", page),
        new CURLParameter("str", _str),
        new CURLParameter("total", getMaxLevelPageSize())
    };
}

std::vector<LevelAPI::DatabaseController::Level *> GDServer_BoomlingsLike19::getLevelsBySearchType(int type, std::string str, int page, std::optional<CurlProxy> proxy) {
    // create curl instance
    auto m_pLinkedCURL = _setupCURL(proxy, _getSecretValueStandard());

    // add parameters
    m_pLinkedCURL->addData(_setupGJLevelsArgs(type, str, page));

    // create url
    std::string url = m_sEndpointURL + "/" + _getLevelListEndpointName();
    // send request to the server
    CURLResult *res = m_pLinkedCURL->access_page(url.c_str(), "POST");

    if (!processCURLAnswer(res)) {
        free((char *)res->data);
        delete res; res = nullptr;
        
        delete m_pLinkedCURL; m_pLinkedCURL = nullptr;

        return {};
    }

    if(res->data[0] == '-') {
        free((char *)res->data);
        delete res; res = nullptr;
        
        delete m_pLinkedCURL; m_pLinkedCURL = nullptr;

        return {};
    }

    // create level array
    std::vector<LevelAPI::DatabaseController::Level *> vec1;

    // split array into level array and player array
    std::vector<std::string> vec2 = splitString(res->data, '#');

    // get player list
    std::string plList = vec2[1];

    // get level list
    std::string lvlList = vec2[0];

    // create player map
    std::map<int, Account19 *> playerMap;

    // split player list into individual players
    std::vector<std::string> vec4array = splitString(plList.c_str(), '|');

    // split level list into individual levels
    std::vector<std::string> vec5levels = splitString(lvlList.c_str(), '|');

    int i = 0;

    std::vector<Account19 *> accounts;

    for (auto player_string : vec4array) {
        // split robtop array
        std::vector<std::string> player_data = splitString(player_string.c_str(), ':');

        if (player_data.size() >= 3) {
            // get strings for each array element;
            std::string userID_string = player_data[0];
            std::string accountID_string = player_data[1];
            std::string username = player_data[2];

            // parse userID
            int userID = atoi(userID_string.c_str());
            // parse accountID
            int accountID = atoi(accountID_string.c_str());

            Account19 *account = new Account19();

            account->accountID = accountID;
            account->userID = userID;
            account->username = username;

            playerMap.insert(std::pair<int, Account19 *>(userID, account));
            accounts.push_back(account);
        }
    }

    for (auto level_string : vec5levels) {
        // parse level
        LevelAPI::DatabaseController::Level *lvl = LevelParser::parseFromResponse(level_string);
        
        // get account by user id
        Account19 *account = playerMap[lvl->m_nPlayerID];

        // set account values if it was found
        if (account != nullptr) {
            lvl->m_nAccountID = account->accountID;
            lvl->m_sUsername = account->username;
        }

        // set level release values
        lvl->m_uRelease->m_nGameVersion = lvl->m_nGameVersion;
        lvl->m_uRelease->m_fActualVersion = determineGVFromID(lvl->m_nLevelID);

        vec1.push_back(lvl);
    }
    
    // delete accounts
    for (auto account : accounts) delete account;

    free((char *)res->data);
    delete res; res = nullptr;
        
    delete m_pLinkedCURL; m_pLinkedCURL = nullptr;

    // reserse array to make first level to be the latest one
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
    // create curl instance
    auto m_pLinkedCURL = _setupCURL(proxy, _getSecretValueStandard());
    // add level id parameter
    m_pLinkedCURL->setData({
        new CURLParameter("levelID", level->m_nLevelID)
    });
    // generate url
    std::string url = m_sEndpointURL + "/" +_getDownloadLevelEndpointName();
    // create request to the server
    CURLResult *res = m_pLinkedCURL->access_page(url.c_str(), "POST");

    if (!processCURLAnswer(res)) {
        free((char *)res->data);
        delete res; res = nullptr;
        
        delete m_pLinkedCURL; m_pLinkedCURL = nullptr;

        return level;
    }

    // check if level doesn't exist
    std::string strtest = res->data;
    if(!strtest.compare("-1")) {
        free((char *)res->data);
        delete res; res = nullptr;
        
        delete m_pLinkedCURL; m_pLinkedCURL = nullptr;

        return level;
    }

    // parse level
    LevelAPI::DatabaseController::Level *lvl = LevelParser::parseFromResponse(res->data);
    
    // set values
    level->m_sLevelString = lvl->m_sLevelString;
    level->m_nMusicID = lvl->m_nMusicID;
    level->m_nSongID = lvl->m_nSongID;
    level->m_uRelease->m_nGameVersion = lvl->m_nGameVersion;
    level->m_uRelease->m_fActualVersion = determineGVFromID(lvl->m_nLevelID);

    free((char *)res->data);
    delete res; res = nullptr;
        
    delete m_pLinkedCURL; m_pLinkedCURL = nullptr;

    delete lvl; lvl = nullptr;

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

GDServerUploadResult *GDServer_BoomlingsLike19::uploadLevel(DatabaseController::Level *level, std::optional<CurlProxy> proxy) {
    return nullptr;
}

std::vector<LevelAPI::DatabaseController::Level *> GDServer_BoomlingsLike19::getLevelsFromResponse(std::string &response) {
    // TODO: implement this
    
    return {};
}