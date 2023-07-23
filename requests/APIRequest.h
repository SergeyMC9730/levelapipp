#pragma once

#include <httpserver.hpp>

#include <string>
#include <string_view>
#include <optional>

#include "HTTPContentType.h"

class APIRequest : public httpserver::http_resource {
public:
    std::shared_ptr<httpserver::http_response> generateResponse(std::string data, int status = 200);
    std::shared_ptr<httpserver::http_response> generateResponse(std::string_view data, int status = 200);
    std::shared_ptr<httpserver::http_response> generateResponse(int data, int status = 200);
    std::shared_ptr<httpserver::http_response> generateResponse(const char *data, int status = 200);
    std::shared_ptr<httpserver::http_response> generateResponse(std::string data, HTTPContentType type, int status = 200);
    std::shared_ptr<httpserver::http_response> generateResponse(std::string_view data, HTTPContentType type, int status = 200);
    std::shared_ptr<httpserver::http_response> generateResponse(int data, HTTPContentType type, int status = 200);
    std::shared_ptr<httpserver::http_response> generateResponse(const char *data, HTTPContentType type, int status = 200);

    std::shared_ptr<httpserver::file_response> sendFile(std::string path, HTTPContentType type);

    std::optional<std::string> getArgument(std::string key, const httpserver::http_request &req);

    const char *request_name;
    const char *request_url;

    httpserver::http_resource *getAsResource();
};