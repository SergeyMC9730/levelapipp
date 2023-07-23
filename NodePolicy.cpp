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