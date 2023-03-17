#include "api_request.h"

std::shared_ptr<httpserver::http_response> APIRequest::generateResponse(std::string data) {
    return std::shared_ptr<httpserver::http_response>(new httpserver::string_response(data));
}
std::shared_ptr<httpserver::http_response> APIRequest::generateResponse(std::string_view data) {
    return APIRequest::generateResponse(std::string(data.data()));
}
std::shared_ptr<httpserver::http_response> APIRequest::generateResponse(const char *data) {
    return APIRequest::generateResponse(std::string(data));
}