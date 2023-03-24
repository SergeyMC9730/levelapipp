#include "api_request.h"

std::shared_ptr<httpserver::http_response> APIRequest::generateResponse(std::string data) {
    return std::shared_ptr<httpserver::http_response>(new httpserver::string_response(data));
}
std::shared_ptr<httpserver::http_response> APIRequest::generateResponse(std::string_view data) {
    return APIRequest::generateResponse(std::string(data.data()));
}
std::shared_ptr<httpserver::http_response> APIRequest::generateResponse(int data) {
    std::string str = "";
    str += data;
    return APIRequest::generateResponse(std::string(str));
}
std::shared_ptr<httpserver::http_response> APIRequest::generateResponse(const char *data) {
    return APIRequest::generateResponse(std::string(data));
}

httpserver::http_resource *APIRequest::getAsResource() {
    return (httpserver::http_resource *)this;
}