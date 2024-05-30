/**
 *  LevelAPI - Geometry Dash level cacher with search functionality and more.
    Copyright (C) 2023  Sergei Baigerov

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

// stub until changes in api would appear

#include "GDServer_BasementLike21.h"
#include "Translation.h"
#include "StringSplit.h"
#include "RobTopStringContainer.hpp"
#include "Level.h"

using namespace LevelAPI::Backend;

GDServer_BasementLike21::GDServer_BasementLike21(std::string endpoint) : GDServer_BoomlingsLike21(endpoint) {}
GDServer_BasementLike21::GDServer_BasementLike21(std::string endpoint, std::vector<LevelRange> list) : GDServer_BoomlingsLike21(endpoint, list) {}
GDServer_BasementLike21::GDServer_BasementLike21(std::string endpoint, LevelRangeList list) : GDServer_BoomlingsLike21(endpoint, list) {}

std::string GDServer_BasementLike21::getServerName() {
    return Frontend::Translation::getByKey("lapi.gdserver_basementlike21.name");
}
std::string GDServer_BasementLike21::getServerIdentifier() {
    return "gdserver_basementlike21";
}

std::string GDServer_BasementLike21::_getSecretValueRates() {
    return "Wmfd3879gb7";
}

std::string GDServer_BasementLike21::_getRateListEndpointName() {
    return "getGJRates21.php";
}

std::vector<LevelAPI::DatabaseController::Level *> GDServer_BasementLike21::getRecentRatedLevels(int list_length, std::optional<CurlProxy> proxy) {
    auto curl = new CURLConnection();

    curl->setDebug(getDebug());

    curl->setData({
        new CURLParameter("secret", _getSecretValueRates()),
        new CURLParameter("gameVersion", getGameVersion()),
        new CURLParameter("size", list_length)
    });

    if (proxy.has_value()) {
        curl->setProxy(proxy.value());
    }

    std::string url = m_sEndpointURL + "/" + _getRateListEndpointName();

    CURLResult *res = curl->access_page(url.c_str(), "POST");

    if (!processCURLAnswer(res)) {
        free((char *)res->data); delete res;

        delete curl;

        return {};
    }

    auto levels = splitString(res->data, ',');

    RobTopStringContainer *container = new RobTopStringContainer("");

    container->setParserForVariable({1, 6, 18, 49, 19, 25, 42}, [&](std::string inputString, int inputID) {
        try {
            return std::stoi(inputString);
        } catch (std::invalid_argument &e) {
            return 0;
        }
    });

    container->setParserForVariable({2, 7}, [&](std::string inputString, int inputID) {
        return inputString;
    });

    std::vector<LevelAPI::DatabaseController::Level *> _levels;

    for (std::string level : levels) {
        container->resetValues();

        container->setString(level);
        container->parse();

        auto lobj = new LevelAPI::DatabaseController::Level("");

        GET_KEY_GKINT(container, 1, lobj->m_nLevelID);
        GET_KEY_GKINT(container, 6, lobj->m_nPlayerID);
        GET_KEY_GKINT(container, 18, lobj->m_nStars);
        GET_KEY_GKINT(container, 49, lobj->m_nAccountID);
        GET_KEY_GKINT(container, 19, lobj->m_nFeatureScore);
        GET_KEY_GKINT(container, 25, lobj->m_bAuto);
        GET_KEY_GKINT(container, 42, lobj->m_nEpic);

        GET_KEY_GKSTRING(container, 2, lobj->m_sLevelName);
        GET_KEY_GKSTRING(container, 7, lobj->m_sUsername);

        _levels.push_back(lobj);
    }

    delete container;

    return _levels;
}