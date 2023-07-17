#include "api_request.h"

std::shared_ptr<httpserver::http_response> APIRequest::generateResponse(std::string data, int status) {
    return std::shared_ptr<httpserver::http_response>(new httpserver::string_response(data, status));
}
std::shared_ptr<httpserver::http_response> APIRequest::generateResponse(std::string_view data, int status) {
    return APIRequest::generateResponse(std::string(data.data()), status);
}
std::shared_ptr<httpserver::http_response> APIRequest::generateResponse(int data, int status) {
    std::string str = std::to_string(data);

    return APIRequest::generateResponse(std::string(str), status);
}
std::shared_ptr<httpserver::http_response> APIRequest::generateResponse(const char *data, int status) {
    return APIRequest::generateResponse(std::string(data), status);
}

std::shared_ptr<httpserver::http_response> APIRequest::generateResponse(std::string data, HTTPContentType type, int status) {
    return std::shared_ptr<httpserver::http_response>(new httpserver::string_response(data, status, type.getType()));
}
std::shared_ptr<httpserver::http_response> APIRequest::generateResponse(std::string_view data, HTTPContentType type, int status) {
    return APIRequest::generateResponse(std::string(data.data()), type, status);
}
std::shared_ptr<httpserver::http_response> APIRequest::generateResponse(int data, HTTPContentType type, int status) {
    std::string str = "";
    str += data;
    return APIRequest::generateResponse(std::string(str), type, status);
}
std::shared_ptr<httpserver::http_response> APIRequest::generateResponse(const char *data, HTTPContentType type, int status) {
    return APIRequest::generateResponse(std::string(data), type);
}

std::shared_ptr<httpserver::file_response> APIRequest::sendFile(std::string path, HTTPContentType type) {
    return std::shared_ptr<httpserver::file_response>(new httpserver::file_response(path, 200, type.getType()));
}

httpserver::http_resource *APIRequest::getAsResource() {
    return dynamic_cast<httpserver::http_resource *>(this);
}