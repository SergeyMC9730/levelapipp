#pragma once

#include <httpserver.hpp>

#include <string>
#include <string_view>

#include "HTTPContentType.h"

class APIRequest : public httpserver::http_resource {
public:
    std::shared_ptr<httpserver::http_response> generateResponse(std::string data);
    std::shared_ptr<httpserver::http_response> generateResponse(std::string_view data);
    std::shared_ptr<httpserver::http_response> generateResponse(int data);
    std::shared_ptr<httpserver::http_response> generateResponse(const char *data);
    std::shared_ptr<httpserver::http_response> generateResponse(std::string data, std::string content_type);
    std::shared_ptr<httpserver::http_response> generateResponse(std::string_view data, std::string content_type);
    std::shared_ptr<httpserver::http_response> generateResponse(int data, std::string content_type);
    std::shared_ptr<httpserver::http_response> generateResponse(const char *data, std::string content_type);

    std::shared_ptr<httpserver::file_response> sendFile(std::string path, HTTPContentType type);

    const char *request_name;
    const char *request_url;

    httpserver::http_resource *getAsResource();
};