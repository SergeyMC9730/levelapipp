#include "APIRequest.h"
#include <optional>

#include "../StringSplit.h"

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
    return APIRequest::generateResponse(std::string(data), type, status);
}

std::shared_ptr<httpserver::file_response> APIRequest::sendFile(std::string path, std::string remote_filename, HTTPContentType type) {
    auto resp = std::shared_ptr<httpserver::file_response>(new httpserver::file_response(path, 200, type.getType()));
    
    resp->with_header("Content-Disposition", "attachment; filename=" + remote_filename);

    return resp;
}
std::shared_ptr<httpserver::file_response> APIRequest::sendFile(std::string path, HTTPContentType type) {
    auto p = splitString(path.c_str(), '/');

    auto filename = p[p.size() - 1];
    
    return sendFile(path, filename, type);
}

std::shared_ptr<httpserver::file_response> APIRequest::sendFile(std::string path, std::string remote_filename) {
    auto resp = std::shared_ptr<httpserver::file_response>(new httpserver::file_response(path, 200));
    
    resp->with_header("Content-Disposition", "attachment; filename=" + remote_filename);

    return resp;
}
std::shared_ptr<httpserver::file_response> APIRequest::sendFile(std::string path) {
    auto p = splitString(path.c_str(), '/');

    auto filename = p[p.size() - 1];
    
    return sendFile(path, filename);
}

httpserver::http_resource *APIRequest::getAsResource() {
    return dynamic_cast<httpserver::http_resource *>(this);
}

std::optional<std::string> APIRequest::getArgument(std::string key, const httpserver::http_request &req) {
    auto data = req.get_arg(key);

    if (data.get_all_values().size() == 0) return std::nullopt;

    std::string data_str = data.get_flat_value().data();

    return data_str;
}