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
#include "json/single_include/nlohmann/json.hpp"

#include "Tools.h"

using namespace LevelAPI::DatabaseController;

NodePolicy::NodePolicy() {
    // init with default settings
    m_bEnableRecentTab = false;
    m_bWaitResolverRL = true;
    m_nResolverInterval = 4;
    m_nQueueProcessingInterval = 2;
    m_bEnableLinearResolver = false;
    m_bEnableResolver = false;
    m_bUseProxyOnly = false;

    setupJSON();
    save();
}

void NodePolicy::save() {
    _jsonObject["enableRecentTab"] = m_bEnableRecentTab;
    _jsonObject["waitResolverRateLimit"] = m_bWaitResolverRL;
    _jsonObject["resolverInterval"] = m_nResolverInterval;
    _jsonObject["queueProcessingInterval"] = m_nQueueProcessingInterval;
    _jsonObject["enableLinearResolver"] = m_bEnableLinearResolver;
    _jsonObject["enableResolver"] = m_bEnableResolver;
    _jsonObject["noOutput"] = m_bNoOutput;
    _jsonObject["useProxyOnly"] = m_bUseProxyOnly;
}
void NodePolicy::recover() {
    GET_JSON_VALUE(_jsonObject, "enableRecentTab", m_bEnableRecentTab, bool);
    GET_JSON_VALUE(_jsonObject, "waitResolverRateLimit", m_bWaitResolverRL, bool);
    GET_JSON_VALUE(_jsonObject, "resolverInterval", m_nResolverInterval, float);
    GET_JSON_VALUE(_jsonObject, "queueProcessingInterval", m_nQueueProcessingInterval, float);
    GET_JSON_VALUE(_jsonObject, "enableLinearResolver", m_bEnableLinearResolver, bool);
    GET_JSON_VALUE(_jsonObject, "enableResolver", m_bEnableResolver, bool);
    GET_JSON_VALUE(_jsonObject, "noOutput", m_bNoOutput, bool);
    GET_JSON_VALUE(_jsonObject, "useProxyOnly", m_bUseProxyOnly, bool);
}

void NodePolicy::setupJSON() {
    _jsonObject = nlohmann::json();
}