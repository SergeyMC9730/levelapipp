#include "v1.level.download.h"

#include "../lapi_database.h"
#include "../Level.h"

#include <stdexcept>
#include <string>
#include <sys/stat.h>
#include <iostream>

#include "../ModuleGDHistory.h"

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
    auto provider = req.get_arg("provider");

    int id2 = 0;

    int type = 0;
    std::string provider_string = "node-cached";

    std::vector<std::string> supportedProviders = {"node-cached"};

    try {
        if(!type1.get_flat_value().empty()) {
            type = std::stoi(std::string(type1));
        }
    } catch (std::invalid_argument &e) {
        type = 0;
    }

    try {
        if(!id.get_flat_value().empty()) {
            id2 = std::stoi(std::string(id));
        }
    } catch (std::invalid_argument &e) {
        id2 = 0;
    }

    if (!provider.get_flat_value().empty()) {
        provider_string = std::string(provider);
    }

    nlohmann::json response_fail;

    response_fail["response"] = -1;
    response_fail["id"] = id;
    response_fail["node"] = node;
    response_fail["provider"] = provider_string;

    auto nodepointer = LevelAPI::DatabaseController::database->getNode(std::string(node));

    if(nodepointer == nullptr) {
        return generateResponse(response_fail.dump(), HTTPContentTypeJSON(), 404);
    }

    auto server = nodepointer->createServer();

    if (std::find(server->getModules().begin(), server->getModules().end(), "gdhistory") != server->getModules().end()) {
        supportedProviders.push_back("gdhistory");
    }

    if (std::find(supportedProviders.begin(), supportedProviders.end(), provider_string) == supportedProviders.end()) {
        response_fail["response"] = -3;
        return generateResponse(response_fail.dump(), HTTPContentTypeJSON(), 404);
    }
    
    switch (type) {
        default: // failback to json
        case 0: { // json
            DatabaseController::Level *level = nullptr;

            if (provider_string == "node-cached") {
                level = nodepointer->getLevel(id2);
            } else if (provider_string == "gdhistory") {
                auto module_gd = dynamic_cast<Backend::ModuleGDHistory *>(server);

                level = module_gd->downloadArchivedLevel(id2);
            }

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
            if (provider_string == "gdhistory") {
                response_fail["response"] = -4;
                return generateResponse(response_fail.dump(), HTTPContentTypeJSON(), 501);
            }

            auto level = nodepointer->getLevel(id2);

            if(level == nullptr) {
                return generateResponse("404 Level Not Found", 404);
            }

            std::string path = level->m_sLevelPath + "/data.gmd2";

            delete level;
            level = nullptr;

            std::string filename = nodepointer->m_sInternalName + "_" + std::string(id.get_flat_value()) + ".gmd2";

            return sendFile(path, filename, HTTPContentTypeText());

            break;
        }
    }
}