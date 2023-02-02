#pragma once

#include <httpserver.hpp>

#define SEND_RESPONSE(txt) return std::shared_ptr<http_response>(new string_response(txt))

class APIRequest : public httpserver::http_resource {
public:
    const char *request_name;
    const char *request_url;
};