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

    auto filter = new LevelAPI::Backend::SearchFilter();

    filter->m_eSort = LevelAPI::Backend::SearchSort::SSLatestDBApperead;
    
    auto levels = node_object->getLevels(filter);

    nlohmann::json resp;

    resp["response"] = 0;
    resp["levels"] = node_object->m_nCachedLevels;
    resp["latestLevelsDownloaded"] = nlohmann::json::array();
    resp["queuedJobs"] = node_object->m_uQueue->m_vCommandQueue.size();
    resp["queuedDownloadJobs"] = node_object->m_uQueue->m_vResolverQueuedLevels.size();

    int i = 0;
    while(i < levels.size()) {
        resp["latestLevelsDownloaded"].push_back(levels[i]->_jsonObject);

        delete levels[i];
        levels[i] = nullptr;

        i++;
    }

    delete filter;
    filter = nullptr;

    return generateResponse(resp.dump(), HTTPContentTypeJSON());
}
