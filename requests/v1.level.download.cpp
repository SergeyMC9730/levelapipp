#include "v1.level.download.h"

#include "../lapi_database.h"
#include <stdexcept>
#include <string>
#include <sys/stat.h>
#include <iostream>

#include "HTTPContentTypeText.h"
#include "HTTPContentTypeJSON.h"
#include "HTTPContentTypeZip.h"

#define file_exists(cstr) (stat(cstr, &buffer) == 0)

LevelAPI::v1::LevelDownloadRequest::LevelDownloadRequest() {
    this->request_name = "download level metadata";
    this->request_url = "/api/v1/level/download";
}

std::shared_ptr<http_response> LevelAPI::v1::LevelDownloadRequest::render(const http_request &req) {
    auto id = req.get_arg("id");
    auto node = req.get_arg("node");
    auto type1 = req.get_arg("type");
    int type = 0;

    try {
        if(!type1.get_flat_value().empty()) {
            type = std::stoi(std::string(type1));
        }
    } catch (std::invalid_argument &e) {
        type = 0;
    }
    
    switch (type) {
        default: // fail to json
        case 0: { // json
            nlohmann::json response_fail;

            response_fail["response"] = -1;
            response_fail["id"] = id;
            response_fail["node"] = node;

            auto nodepointer = LevelAPI::DatabaseController::database->getNode(std::string(node));

            if(nodepointer == nullptr) {
                return generateResponse(response_fail.dump(), HTTPContentTypeJSON(), 404);
            }

            auto level = nodepointer->getLevel(std::stoi(std::string(id)));

            if(level == nullptr) {
                response_fail["response"] = -2;
                return generateResponse(response_fail.dump(), HTTPContentTypeJSON(), 404);
            }

            level->_jsonObject["response"] = 0U;
            level->_jsonObject["hasLevelString"] = level->m_bHasLevelString;
            
            auto resp = generateResponse(level->_jsonObject.dump(-1, ' ', false, nlohmann::json::error_handler_t::ignore), HTTPContentTypeJSON());
            
            delete level;
            level = nullptr;

            return resp;

            break;
        }
        case 1: { // gmd2
            auto nodepointer = LevelAPI::DatabaseController::database->getNode(std::string(node));

            if(nodepointer == nullptr) {
                return generateResponse("404 Node Not Found", 404);
            }

            auto level = nodepointer->getLevel(std::stoi(std::string(id)));

            if(level == nullptr) {
                return generateResponse("404 Level Not Found", 404);
            }

            std::string path = level->m_sLevelPath + "/data.gmd2";

            delete level;
            level = nullptr;

            return sendFile(path, HTTPContentTypeZip());

            break;
        }
    }
}