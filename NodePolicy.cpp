#include "lapi_database.h"
#include "json/single_include/nlohmann/json.hpp"

#include "Tools.h"

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
    policyJson["noOutput"] = m_bNoOutput;
}
void NodePolicy::recover() {
    GET_JSON_VALUE(policyJson, "enableRecentTab", m_bEnableRecentTab, bool);
    GET_JSON_VALUE(policyJson, "waitResolverRateLimit", m_bWaitResolverRL, bool);
    GET_JSON_VALUE(policyJson, "resolverInterval", m_nResolverInterval, float);
    GET_JSON_VALUE(policyJson, "queueProcessingInterval", m_nQueueProcessingInterval, float);
    GET_JSON_VALUE(policyJson, "enableLinearResolver", m_bEnableLinearResolver, bool);
    GET_JSON_VALUE(policyJson, "enableResolver", m_bEnableResolver, bool);
    GET_JSON_VALUE(policyJson, "noOutput", m_bNoOutput, bool);
}

void NodePolicy::setupJSON() {
    policyJson = nlohmann::json();
}