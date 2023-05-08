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

std::shared_ptr<httpserver::http_response> APIRequest::generateResponse(std::string data, std::string content_type) {
    return std::shared_ptr<httpserver::http_response>(new httpserver::string_response(data, 200, content_type));
}
std::shared_ptr<httpserver::http_response> APIRequest::generateResponse(std::string_view data, std::string content_type) {
    return APIRequest::generateResponse(std::string(data.data()), content_type);
}
std::shared_ptr<httpserver::http_response> APIRequest::generateResponse(int data, std::string content_type) {
    std::string str = "";
    str += data;
    return APIRequest::generateResponse(std::string(str), content_type);
}
std::shared_ptr<httpserver::http_response> APIRequest::generateResponse(const char *data, std::string content_type) {
    return APIRequest::generateResponse(std::string(data), content_type);
}

std::shared_ptr<httpserver::file_response> APIRequest::sendFile(std::string path) {
    return std::shared_ptr<httpserver::file_response>(new httpserver::file_response(path, 200, "image/png"));
}

httpserver::http_resource *APIRequest::getAsResource() {
    return (httpserver::http_resource *)this;
}