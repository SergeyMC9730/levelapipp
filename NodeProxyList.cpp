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

#include "lapi_database.h"
#include <iostream>

using namespace LevelAPI::DatabaseController;

NodeProxyList::NodeProxyList() {
    setupJSON();
    save();
}

void NodeProxyList::setupJSON() {
    _jsonObject = nlohmann::json();
}

void NodeProxyList::save() {
    _jsonObject["list"] = m_vProxies;
}
void NodeProxyList::recover() {
    for (auto el = _jsonObject["list"].begin(); el != _jsonObject["list"].end(); ++el) {
        m_vProxies.push_back(el->get<std::string>());
        std::cout << "proxy " << el->get<std::string>() << std::endl;
    }
}