#include "v1.node.queue.h"

#include "../lapi_database.h"
#include <string>
#include <sys/stat.h>
#include <iostream>

#include "HTTPContentTypeJSON.h"

#define file_exists(cstr) (stat(cstr, &buffer) == 0)

LevelAPI::v1::QueueAddRequest::QueueAddRequest() {
    this->request_name = "request a task for node";
    this->request_url = "/api/v1/node/queue";
}

std::shared_ptr<http_response> LevelAPI::v1::QueueAddRequest::render(const http_request &req) {
    std::string node = req.get_arg_flat("node").data();

    auto node_object = DatabaseController::database->getNode(node);

    if (node_object == nullptr) {
        nlohmann::json response_fail;

        response_fail["response"] = -1;
        response_fail["node"] = node;

        return generateResponse(response_fail.dump(), HTTPContentTypeJSON(), 404);
    }

    std::vector<unsigned char> valid_tasks = {
        DatabaseController::NC_ID, DatabaseController::NC_USER,
        DatabaseController::NC_RECENT, DatabaseController::NC_EXPERIMENT1,
        DatabaseController::NC_NONE, DatabaseController::NC_IDLE,
        DatabaseController::NC_EXPERIMENT2, DatabaseController::NC_22REGION_META
    };

#define PARSE_VAL(key, type, value) try { data = req.get_arg_flat(key).data(); if (!data.empty()) value = (type)std::stoi(data); } catch (std::invalid_argument invarg) {}
    std::string data = "";

    unsigned char type = DatabaseController::NC_NONE;
    std::string data1 = req.get_arg_flat("data").data();

    PARSE_VAL("type", unsigned char, type);

    if (std::find(valid_tasks.begin(), valid_tasks.end(), type) == valid_tasks.end()) {
        nlohmann::json response_fail;

        response_fail["response"] = -2;
        response_fail["type"] = (int)type;

        return generateResponse(response_fail.dump(), HTTPContentTypeJSON(), 300);
    }
    
    node_object->m_uQueue->m_vCommandQueue.push_back({type, data1});

    nlohmann::json response;

    response["response"] = 0;
    response["amount"] = node_object->m_uQueue->m_vCommandQueue.size();
    response["type"] = (int)type;
    response["data"] = data1;

    return generateResponse(response.dump(), HTTPContentTypeJSON(), 200);
}