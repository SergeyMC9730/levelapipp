#pragma once

#include <httpserver.hpp>

#include <string>
#include <string_view>

class APIRequest : public httpserver::http_resource {
public:
    std::shared_ptr<httpserver::http_response> generateResponse(std::string data);
    std::shared_ptr<httpserver::http_response> generateResponse(std::string_view data);
    std::shared_ptr<httpserver::http_response> generateResponse(int data);
    std::shared_ptr<httpserver::http_response> generateResponse(const char *data);

    const char *request_name;
    const char *request_url;

    httpserver::http_resource *getAsResource();
};