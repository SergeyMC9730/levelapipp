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
#include "Tools.h"

using namespace LevelAPI::DatabaseController;

NodeDatabase::NodeDatabase(std::string endpoint, uint8_t featureSet, bool readOnly) {
    m_sEndpoint = endpoint;
    m_nFeatureSet = featureSet;
    m_bReadOnly = readOnly;

    setupJSON();

    save();
}
NodeDatabase::NodeDatabase() {
    m_nFeatureSet = 21;
    m_bReadOnly = true;

    setupJSON();
    save();
}

void NodeDatabase::save() {
    _jsonObject["endpoint"] = m_sEndpoint;
    _jsonObject["featureSet"] = m_nFeatureSet;
    _jsonObject["readOnly"] = m_bReadOnly;
    _jsonObject["playerLogin"] = m_sPlayerLogin;
    _jsonObject["playerPassword"] = m_sPlayerPassword;
    if(!m_sModifications.empty()) _jsonObject["modifications"] = m_sModifications;
}

void NodeDatabase::recover() {
    GET_JSON_VALUE(_jsonObject, "endpoint", m_sEndpoint, std::string);
    GET_JSON_VALUE(_jsonObject, "featureSet", m_nFeatureSet, int);
    GET_JSON_VALUE(_jsonObject, "readOnly", m_bReadOnly, bool);
    GET_JSON_VALUE(_jsonObject, "modifications", m_sModifications, std::string);
    GET_JSON_VALUE(_jsonObject, "playerLogin", m_sPlayerLogin, std::string);
    GET_JSON_VALUE(_jsonObject, "playerPassword", m_sPlayerPassword, std::string);
}

void NodeDatabase::setupJSON() {
    _jsonObject = nlohmann::json();
}

NodeDatabase::~NodeDatabase() {}

std::vector<std::string> NodeDatabase::getModifications() {
    // wip
    return {m_sModifications};
}

std::vector<std::string> NodeDatabase::getModificationsList() {
    return {"boomlings", "basement-custom", "19gdps-custom"};
}
std::vector<int> NodeDatabase::getFeatureSetsList() {
    return {19, 21, 22};
}