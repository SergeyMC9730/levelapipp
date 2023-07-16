#include "lapi_database.h"

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