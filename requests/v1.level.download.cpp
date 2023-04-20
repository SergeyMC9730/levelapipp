#include "v1.level.download.h"

#include "../lapi_database.h"
#include <string>
#include <sys/stat.h>
#include <iostream>

#define file_exists(cstr) (stat(cstr, &buffer) == 0)

LevelAPI::v1::LevelDownloadRequest::LevelDownloadRequest() {
    this->request_name = "download level metadata";
    this->request_url = "/api/v1/level/download";
}

std::shared_ptr<http_response> LevelAPI::v1::LevelDownloadRequest::render(const http_request &req) {
    auto id = req.get_arg("id");
    auto node = req.get_arg("node");
    auto isgmd2 = req.get_arg("gmd2");

    nlohmann::json response_fail;

    response_fail["response"] = -1;
    response_fail["id"] = id;
    response_fail["node"] = node;

    auto nodepointer = LevelAPI::DatabaseController::database->getNode(std::string(node));

    if(nodepointer == nullptr) {
        return generateResponse(response_fail.dump());
    }

    auto level = nodepointer->getLevel(std::stoi(std::string(id)));

    if(level == nullptr) {
        response_fail["response"] = -2;
        return generateResponse(response_fail.dump());
    }

    level->levelJson["response"] = 0U;
    
    auto resp = generateResponse(level->levelJson.dump(-1, ' ', false, nlohmann::json::error_handler_t::ignore), "application/json");
    
    delete level;
    level = nullptr;

    return resp;
}