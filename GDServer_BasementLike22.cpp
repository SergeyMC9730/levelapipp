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

#include "GDServer_BasementLike22.h"
#include "Account19.h"

using namespace LevelAPI::Backend;

GDServer_BasementLike22::GDServer_BasementLike22(std::string endpoint) : GDServer_BasementLike21(endpoint) {
    m_pAccount = new Account19();
}
GDServer_BasementLike22::GDServer_BasementLike22(std::string endpoint, std::vector<LevelRange> list) : GDServer_BasementLike21(endpoint, list) {
    m_pAccount = new Account19();
}
GDServer_BasementLike22::GDServer_BasementLike22(std::string endpoint, LevelRangeList list) : GDServer_BasementLike21(endpoint, list) {
    m_pAccount = new Account19();
}

std::vector<CURLParameter *> GDServer_BasementLike22::_setupGJLevelsArgs(int type, std::string str, int page) {
    auto vec = GDServer_BasementLike21::_setupGJLevelsArgs(type, str, page);

    vec.push_back(new CURLParameter("diff", "-"));
    vec.push_back(new CURLParameter("len", "-"));
    vec.push_back(new CURLParameter("uncompleted", "-"));
    vec.push_back(new CURLParameter("onlyCompleted", "-"));
    vec.push_back(new CURLParameter("featured", "-"));
    vec.push_back(new CURLParameter("original", "-"));
    vec.push_back(new CURLParameter("twoPlayer", "-"));
    vec.push_back(new CURLParameter("coins", "-"));

    return vec;
}

std::string GDServer_BasementLike22::getServerIdentifier() {
    return "gdserver_basementlike22";
}

int GDServer_BasementLike22::getGameVersion() {
    return 22;
}

#include "UUID.h"

CURLConnection *GDServer_BasementLike22::_setupCURL(std::optional<CurlProxy> proxy, std::string secret) {
    auto con = GDServer_BasementLike21::_setupCURL(proxy, secret);

    con->addData({
        new CURLParameter("binaryVersion", getGameVersion()), // TEMP
        new CURLParameter("udid", "podval"),
        new CURLParameter("accountID", m_pAccount->accountID),
        new CURLParameter("gjp2", "a"), // im lazy to implement gjp2
        new CURLParameter("uuid", ConnectionCrypt::createUUID())
    });

    return con;
}