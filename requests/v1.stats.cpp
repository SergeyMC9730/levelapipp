#include "v1.stats.h"

#include "../lapi_database.h"

#include "HTTPContentTypeJSON.h"

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

    auto filter = new SearchFilter();

    filter->m_eSort = SearchSort::SSLatestDBApperead;
    
    auto levels = node_object->getLevels(filter);
    
    return generateResponse("wip");
}