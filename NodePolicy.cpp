#include "lapi_database.h"
#include "json/single_include/nlohmann/json.hpp"

#include <iostream>

using namespace LevelAPI::DatabaseController;

NodePolicy::NodePolicy() {
    // init with default settings
    m_bEnableRecentTab = false;
    m_bWaitResolverRL = true;
    m_nResolverInterval = 4;
    m_nQueueProcessingInterval = 2;
    m_bEnableLinearResolver = false;
    m_bEnableResolver = false;

    setupJSON();
    save();
}

void NodePolicy::save() {
    policyJson["enableRecentTab"] = m_bEnableRecentTab;
    policyJson["waitResolverRateLimit"] = m_bWaitResolverRL;
    policyJson["resolverInterval"] = m_nResolverInterval;
    policyJson["queueProcessingInterval"] = m_nQueueProcessingInterval;
    policyJson["enableLinearResolver"] = m_bEnableLinearResolver;
    policyJson["enableResolver"] = m_bEnableResolver;
}
void NodePolicy::recover() {
    if(policyJson.contains("enableRecentTab")) m_bEnableRecentTab = policyJson["enableRecentTab"].get<bool>();
    if(policyJson.contains("waitResolverRateLimit")) m_bWaitResolverRL = policyJson["waitResolverRateLimit"].get<bool>();
    if(policyJson.contains("resolverInterval")) m_nResolverInterval = policyJson["resolverInterval"].get<float>();
    if(policyJson.contains("queueProcessingInterval")) m_nQueueProcessingInterval = policyJson["queueProcessingInterval"].get<float>();
    if(policyJson.contains("enableLinearResolver")) m_bEnableLinearResolver = policyJson["enableLinearResolver"].get<bool>();
    if(policyJson.contains("enableResolver")) m_bEnableResolver = policyJson["enableResolver"].get<bool>();
}

void NodePolicy::setupJSON() {
    policyJson = nlohmann::json();
}