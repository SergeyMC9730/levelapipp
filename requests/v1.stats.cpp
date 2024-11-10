#include "v1.stats.h"

#include "../lapi_database.h"
#include "../Level.h"

#include "HTTPContentTypeJSON.h"

#include "../Level.h"

LevelAPI::v1::StatsRequest::StatsRequest() {
    this->request_name = "get node statistics";
    this->request_url = "/api/v1/stats";
}

std::shared_ptr<http_response> LevelAPI::v1::StatsRequest::render(const http_request &req) {
    auto node = req.get_arg("node");
    std::string node_string = node.get_flat_value().data();

    auto node_object = DatabaseController::database->getNode(node_string);

    if (node_object == nullptr) {
        nlohmann::json response_fail;

        response_fail["response"] = -1;
        response_fail["node"] = node_string;

        return generateResponse(response_fail.dump(), HTTPContentTypeJSON(), 404);
    }

    bool no_levels = false;
    std::string data = "";
    nlohmann::json resp;
    resp["response"] = 0;

#define PARSE_VAL(key, type, value) try { data = req.get_arg_flat(key).data(); if (!data.empty()) value = (type)std::stoi(data); } catch (std::invalid_argument invarg) {}

    PARSE_VAL("noLevels", bool, no_levels);

    if (!no_levels) {
        LevelAPI::Backend::SearchFilter filter;
        filter.m_eSort = LevelAPI::Backend::SearchSort::SSLatestDBApperead;
        
        auto levels = node_object->getLevels(filter);
    
        resp["latestLevelsDownloaded"] = nlohmann::json::array();

        for (auto lvl : levels) {
            resp["latestLevelsDownloaded"].push_back(lvl->_jsonObject);
        }

        Backend::GDServer::destroyLevelVector(levels);
    }
    
    resp["levels"] = node_object->m_nCachedLevels;
    resp["queuedJobs"] = node_object->m_uQueue->m_vCommandQueue.size();
    resp["queuedDownloadJobs"] = node_object->m_uQueue->m_vResolverQueuedLevels.size();

    return generateResponse(resp.dump(), HTTPContentTypeJSON());
}
