#include "lapi_database.h"

using namespace LevelAPI::DatabaseController;

NodeProxyList::NodeProxyList() {
    setupJSON();
    save();
}

void NodeProxyList::setupJSON() {
    plJson = nlohmann::json();
}

void NodeProxyList::save() {
    plJson["list"] = m_vProxies;
}
void NodeProxyList::recover() {
    for (auto el = plJson["list"].begin(); el != plJson["list"].end(); ++el) {
        m_vProxies.push_back(el->get<std::string>());
        std::cout << "proxy " << el->get<std::string>() << std::endl;
    }
}