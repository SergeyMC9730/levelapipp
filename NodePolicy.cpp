#include "lapi_database.h"
#include "json/single_include/nlohmann/json.hpp"

#include <iostream>

using namespace LevelAPI::DatabaseController;

NodePolicy::NodePolicy() {
    // init with default settings
    m_bEnableRecentTab = false;
    m_bWaitResolverRL = true;
    
    setupJSON();
    save();
}

void NodePolicy::save() {
    policyJson["enableRecentTab"] = m_bEnableRecentTab;
    policyJson["waitResolverRateLimit"] = m_bWaitResolverRL;
}
void NodePolicy::recover() {
    m_bEnableRecentTab = policyJson["enableRecentTab"].get<bool>();
    m_bWaitResolverRL = policyJson["waitResolverRateLimit"].get<bool>();

    if(!m_bWaitResolverRL) {
        std::cout << "[LevelAPI WARN] Ignoring Cloudflare's rate limits may cause your IP address being blocked entirely!" << std::endl;
    }
}

void NodePolicy::setupJSON() {
    policyJson = nlohmann::json();
}